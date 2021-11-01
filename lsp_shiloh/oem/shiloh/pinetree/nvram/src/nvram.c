/*
 * ============================================================================
 * Copyright (c) 2008-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_api.c
 *
 *  \brief NVM Manager API implementation file
 *
 *  The Non-Volatile Memory Manager handles the storage, retrieval and 
 *  initialization of non-volatile variables. 
 *
 *  Please see nvram_api.h for interface/usage documentation.
 *
 **/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "posix_ostools.h"
#include "ATypes.h"
#include "error_types.h"
#include "memAPI.h"
#include "dprintf.h"
#include "nvram_logger.h"
#include "debug.h"
#include "checksum.h"
#include "lassert.h"
#include "list.h"
#include "nvram_dev_shim.h"
#include "nvram_api.h"
#include "memdevice.h"
//#include "partitionMgrAPI.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif
#include "utils.h"
#include <ctype.h>
#include "db_api.h"
#include "nvram_db_api.h"

/*
 *  Local Defines
 */
#define NVRAM_STACK_SIZE     POSIX_MIN_STACK_SIZE    /**< NVRAM Write thread stack size     */
#define NVRAM_WRITE_EVENT    1       /**< Write thread event trigger        */
#define ACTIVE_WR            true    /**< block actively writing to device  */
#define NOT_ACTIVE_WR        false   /**< block waiting in write queue      */



/**
 *  \brief NV Write queue entry
 *
 *  NV storage writes are placed into a queue (linked list) using this 
 *  structure.  
 **/
typedef struct NV_WRITE_Q_s
{
    nvram_handle_t   Instance;         /**< Queued copy of instance struct   */
    uint32_t      *DataBuff;         /**< Pointer to the write data buffer */
    bool           activeWrite;      /**< block actively being written     */
    
    ATLISTENTRY    listNode;         /**< Queue linked list pointers       */
} NV_WRITE_Q_t;


/**
 *  \brief Init function list entry
 *
 *  Each module/block owner must provide an init function pointer when 
 *  registering blocks: a list of these structures stores the init function
 *  pointers.
 **/
typedef struct NV_INIT_FUNC_Q_s
{
    char          ModuleLbl[4];        /**< Module/Block Label              */
    nvram_init_func_ptr NvramInitFunction;   /**< NV init function pointer        */
    void           *userData;       /**< Opaque user data */
    nvram_handle_t *blockHandle;    /**< handle passed back to user */

    ATLISTENTRY   listNode;            /**< Init func linked list pointers  */
} NV_INIT_FUNC_Q_t;


/*
 *  Local (static) variables
 */
static pthread_t            nvram_thd_id;
static sem_t                nvram_event_semaphore;
static sem_t                nvram_init_funclist_semaphore; 
static sem_t                nvram_wait_semaphore; 
static pthread_mutex_t      nvram_global_lock;
static unsigned char        NVRAMStack[NVRAM_STACK_SIZE]ALIGN8;
static ATLISTENTRY          NVRAMWriteList;     /**< list for queued NV writes        */
static ATLISTENTRY          NVRAMInitFuncList;  /**< list for nvraminitfunc pointers  */

/*
 *  Local (static) function prototypes
 */
void *NVRAMWriteThread(void *unused);
static error_type_t      NVMWrite(nvram_handle_t *nHandle, int8_t *Src);
static NV_WRITE_Q_t     *FindWriteListEntry(char *BlockLbl, bool activeFlag, ATLISTENTRY *List);
static NV_WRITE_Q_t     *makeActiveWriteEntry(void);
static NV_WRITE_Q_t     *popActiveWriteEntry(void);
static NV_INIT_FUNC_Q_t *FindInitFuncEntry(char *ModuleLbl, ATLISTENTRY *PartList);
static void              SetNvInitFuncPtr(char *ModuleLbl, nvram_init_func_ptr NvramInitFunction, 
                                          void *userData, nvram_handle_t *handle);

typedef enum
{
    e_no_header,
    e_get_header
} get_type_t;

error_type_t nvram_get_var(nvram_handle_t *nHandle, void *DestLocation, get_type_t header);

#ifdef HAVE_CMD
static void nvram_register_cmds(void);
#ifndef HAVE_NVRAM_FILE
void bootspi_cmd_init(void);
#endif
#endif
/*
 * FUNCTION NAME: NvramSysInit 
 *
 * Please see nvram_api.h for function documentation.
 */
error_type_t nvram_sys_init(void)
{
    int px_status;
    error_type_t status;

    /* Create semaphore for NVRAM write thread */
    px_status = posix_mutex_init( &nvram_global_lock );
    XASSERT( px_status==0, px_status );

    px_status = sem_init( &nvram_init_funclist_semaphore, 0, 1 );
    XASSERT( px_status != -1, px_status );

    /* Create semaphore for NVRAM write wait thread */
    px_status = sem_init( &nvram_wait_semaphore, 0, 1 );
    XASSERT( px_status != -1, px_status );

    /* Initialize the linked list for queued write data buffers */
    ATInitList(&NVRAMWriteList);

    /* Initialize the linked list for initialization function pointers */
    ATInitList(&NVRAMInitFuncList);

    /* Create event group */
    px_status = sem_init( &nvram_event_semaphore, 0, 1 );
    XASSERT( px_status != -1, px_status );

    /* Initalize the device shim layer (it sits between the NVRAM API and the
     * Partition Manager).  This will validate the container partition and 
     * discover any existing NVRAM variable blocks.  Note that the shim layer
     * will bind the NV store to a specific memory device.
     */
    status = InitNvramDevShim();

    /* Create thread for writing data on NVRAM memory device */
	px_status = posix_create_thread( &nvram_thd_id, 
                                  NVRAMWriteThread, 
                                  0, 
                                  "NVRAM_write_thread",
                                  NVRAMStack, 
                                  NVRAM_STACK_SIZE, 
                                  POSIX_THR_PRI_LOW);
    XASSERT( px_status==0, px_status );

#ifdef HAVE_CMD
#ifndef HAVE_NVRAM_FILE
    bootspi_cmd_init();
#endif    
    nvram_register_cmds();
#endif

    return status;
}


/*
 * FUNCTION NAME: nvram_variable_register 
 *
 * Please see nvram_api.h for function documentation.
 */
error_type_t nvram_variable_register(nvram_handle_t  **Handle,
                                     char           *Label,
                                     uint16_t        Version,
                                     uint32_t        VarBlockLength,
                                     nvram_init_func_ptr   NvramInitFunction,
                                     void            *UserData)
{
    VARBLOCK_HEADER_t  *varBlock  = NULL;
    nvram_handle_t     *nHandle   = NULL;
    nvram_init_type_t  eInitType;
    error_type_t       blockExists;
    bool               register_new_block = false;
    error_type_t       init_result;
    uint32_t InitLocationSize;
#ifdef DEBUG
    int px_status;
#endif

    /* Make sure we were provided with sane input parameters */
    if ((Handle == NULL) || (Label == NULL) || (NvramInitFunction == NULL) || (VarBlockLength == 0))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): input parameter error\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): register called for '%4s'\n", __LINE__, Label);

#ifdef DEBUG
    // Now see if this label is already registered, if so assert.
    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );
    if(FindInitFuncEntry(Label, &NVRAMInitFuncList) != NULL){
    	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): register called for '%4s'\n", __LINE__, Label);
    	ASSERT(FindInitFuncEntry(Label, &NVRAMInitFuncList) == NULL);
    }
    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );
#endif

    /* NVRAM API clients are given a handle to their variable blocks: allocate
     * memory for a new NVRAM variable instance
     */
    nHandle = (nvram_handle_t *)MEM_MALLOC(sizeof(nvram_handle_t));
    ASSERT(NULL != nHandle);

    /* Fill out the new nvram instance: only the label is required at this point */
    memset(nHandle, 0, sizeof(nvram_handle_t));
    memcpy(&nHandle->BlockLabel[0], Label, sizeof(nHandle->BlockLabel));
#ifdef HAVE_NVRAM_FILE
    nHandle->fd = -1;
#endif

    /* Call the device shim layer to search for the variable block. If found, 
     * LocateVarBlock will fill out the block size and memdev in nHandle.
     */
    blockExists = LocateVarBlock(nHandle);

    if(OK == blockExists)
    {
        // found an existing version of the block; first make sure MaxLength is sane, although
        // all we can do is verify that it is not smaller than the size of the header
        if(nHandle->MaxLength <= sizeof(VARBLOCK_HEADER_t))
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid MaxLength value\n", __LINE__);
            
            // MaxLength is too small, bail
            goto FAIL_EXIT;
        }
       
        // allocate space to read in the existing block 
        varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(nHandle->MaxLength);
        ASSERT(NULL != varBlock);
        InitLocationSize = nHandle->MaxLength - sizeof(VARBLOCK_HEADER_t);

        // read the block in
        if (VarBlockRead(nHandle, (int8_t *)varBlock, UINT64_C(0), nHandle->MaxLength) != nHandle->MaxLength)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to read block\n", __LINE__);
            
            /* Data read failed, release resources and bail out */
            goto FAIL_EXIT;
        }

        // Calculate CRC on Data portion of block, but first make sure Len is sane with respect to MaxLength
        // so the CRC doesn't take forever or read from memory we don't own.  Note that this construct is relying
        // on short-circuit evaluation to ensure CalcChecksum is called only if Len is sane. 
        if ((varBlock->Len > (nHandle->MaxLength - sizeof(VARBLOCK_HEADER_t) + sizeof(VARBLOCK_DATA_t))) || 
            (varBlock->CRC != CalcChecksum( (uint32_t *)&varBlock->Data, sizeof(varBlock->Data) + varBlock->Len)))
        {
            // Len is bad or CRC failed, so the block is bad; release it and let the user know to reset to defaults
            if (ReleaseVarBlock(nHandle) != OK)
            {
                DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to release block\n", __LINE__);
                goto FAIL_EXIT;
            }

            // free the current varBlock and allocate space for the new one
            MEM_FREE_AND_NULL(varBlock);
            varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(sizeof(VARBLOCK_HEADER_t) + VarBlockLength);
            ASSERT(NULL != varBlock);
            InitLocationSize = VarBlockLength;

            varBlock->Data.Version = Version;
            register_new_block = true;
            eInitType = NVRAM_ERROR;
        }
        else
        {
            // block is good; check if the incoming version number changed
            if(varBlock->Data.Version != Version)
            {
                // check the length to see if we can reuse the existing block
                if(varBlock->Len != VarBlockLength)
                {
                    if(VarBlockLength > varBlock->Len)
                    {
                        VARBLOCK_HEADER_t *temp_var_block;

                        // the new block is bigger than the old, so allocate space for the new one and copy the existing block into the new one
                        // (so the caller can migrate old version to new, if desired)
                        temp_var_block = (VARBLOCK_HEADER_t *)MEM_MALLOC(sizeof(VARBLOCK_HEADER_t) + VarBlockLength);
                        ASSERT(NULL != temp_var_block);
                        InitLocationSize = VarBlockLength;
                        ASSERT(nHandle->MaxLength <= (sizeof(VARBLOCK_HEADER_t) + VarBlockLength));
                        memcpy(temp_var_block, varBlock, nHandle->MaxLength);
                        MEM_FREE_AND_NULL(varBlock);
                        varBlock = temp_var_block;
                    }

                    // release current block
                    if (ReleaseVarBlock(nHandle) != OK)
                    {
                        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to release block\n", __LINE__);
                        goto FAIL_EXIT;
                    }

                    register_new_block = true;
                }

                // versions are different
                eInitType = NVRAM_VERSION_CHANGE;
            }
            else if (VarBlockLength != varBlock->Len)
            {
                // If we get here, they have a valid block with a matching version, but the module
                // is reporting its length is different that what was stored.
                // This may occur because someone modified their block without changing the
                // version number, but it can also occur because someone switched between build
                // types on the same formatter.
                // WARN the user that this is bad... then treat it like a version change
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "\n\n");
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "************************************* WARNING *************************************\n");
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, " %s(%d):\n",  __FILE__, __LINE__);
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "   Length for block '%4s' changed (was %d, now %d), but version number (%d) didn't.\n", nHandle->BlockLabel, varBlock->Len, VarBlockLength, Version);
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "***********************************************************************************\n");
                DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "\n\n");

                // release current block
                if (ReleaseVarBlock(nHandle) != OK)
                {
                    goto FAIL_EXIT;
                }

                // free the current varBlock and allocate space for the new one
                MEM_FREE_AND_NULL(varBlock);
                varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(sizeof(VARBLOCK_HEADER_t) + VarBlockLength);
                ASSERT(NULL != varBlock);
                InitLocationSize = VarBlockLength;

                varBlock->Data.Version = Version;
                register_new_block = true;
                eInitType = NVRAM_ERROR;
            }
            else
            {
                // the block exists, the CRC is good, the version was the same, and the lengths match
                eInitType = NVRAM_OK;
            }
        }
    }
    else
    {
        //block was not found, so create a new one
        varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(sizeof(VARBLOCK_HEADER_t) + VarBlockLength);
        ASSERT(NULL != varBlock);
        InitLocationSize = VarBlockLength;

        varBlock->Data.Version = Version;
        register_new_block = true;
        eInitType = NVRAM_NEW;
    }

    /* Call user init function */
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): call init function for '%4s', initType=%d\n",  __LINE__, nHandle->BlockLabel, eInitType);

    // call init function
    init_result = (NvramInitFunction)(eInitType, varBlock->Data.Version, (void *)varBlock->Data.Vars, InitLocationSize, UserData);

    if(OK != init_result)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): NvramInitFunction returned failure\n", __LINE__);
        goto FAIL_EXIT;
    }

    // if required, register the new block
    if(true == register_new_block)
    {
        nHandle->MaxLength = sizeof(VARBLOCK_HEADER_t) + VarBlockLength;
        if (OK != RegisterVarBlock(nHandle))
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to release block\n", __LINE__);
            goto FAIL_EXIT;
        }
    }

    if (eInitType != NVRAM_OK)
    {
        // update header information and calculate new checksum
        varBlock->Data.Version = Version;
        varBlock->Len     = VarBlockLength;

        // include Version + Vars in checksum
        varBlock->CRC = CalcChecksum( (uint32_t *)&varBlock->Data, sizeof(varBlock->Data) + varBlock->Len);

        /* Write the var block back to NV memory using background thread.
         * Note that NVMWrite will make it's own copy of the data.
         */
        if (OK != NVMWrite(nHandle, (int8_t *)varBlock))
        {
            /* Queue data write failed */
            goto FAIL_EXIT;
        }
    }
    /* All went well if we made it here:
     *  - release the temporary variable storage
     *  - set the NV init function pointer 
     *  - return the handle to the caller
     */
    MEM_FREE_AND_NULL(varBlock);
    SetNvInitFuncPtr(nHandle->BlockLabel, NvramInitFunction, UserData, nHandle);
    *Handle = nHandle;
    return OK;


    /* Failure case: Clean up and exit */
    FAIL_EXIT:
    /* Null out the return handle */
    *Handle = NULL;

    if (nHandle != NULL)
    {
        MEM_FREE_AND_NULL(nHandle);
    }

    if (varBlock != NULL)
    {
        MEM_FREE_AND_NULL(varBlock);
    }

    return FAIL;
}

error_type_t db_variable_register(char* Label,
                                  uint16_t Version,
                                  nvram_init_func_ptr   NvramInitFunction)
{
    error_type_t ret = FAIL;
    nvram_init_type_t  eInitType = NVRAM_OK;
    error_type_t init_result = FAIL;

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): DB register called for '%4s'\n", __LINE__, Label);

    // DB Initialize
    ret = db_initialize();

    /* Call user init function */
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): call init function for '%4s', initType=%d\n",  __LINE__, Label, eInitType);

    // call init function
    init_result = (NvramInitFunction)(eInitType, Version, NULL, 0, NULL);

    if(OK != init_result)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): NvramInitFunction returned failure\n", __LINE__);
        ret = FAIL;
    }

    return ret;
}


void nvram_variable_unregister(nvram_handle_t *nHandle)
{
    uint32_t i;
    ATLISTENTRY      *searchNode;
    NV_INIT_FUNC_Q_t *myNode;
    int px_status;

    if(nHandle == NULL)
        return;
    //
    // get rid of the init function from the list.
    //
    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );
     
    // Get the first item off of the registered list.
    searchNode = ATListHead(&NVRAMInitFuncList);    // the list of the registered variable blocks.

    for(i = 0; i < ATNumListNodes(&NVRAMInitFuncList); i++)
    {

        myNode = CONTAINING_RECORD(searchNode, NV_INIT_FUNC_Q_t, listNode);
        if(myNode->blockHandle == nHandle)  // is this the one we want?
        {
            // Yes it is, get it off the list and free the memory.
            //

            DBG_PRINTF(LOG_INFO|DBG_NVRAM, "Found handle %x, remove it\n", myNode->blockHandle); 
            ATRemoveEntryList(searchNode);

            memFree(myNode);
            break;  // done here, go on.
        }

        // get the next entry in the list.
        //
        searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
    }
    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    memFree(nHandle);
}


/*
 * FUNCTION NAME: NvramSetVar 
 *
 * Please see nvram_api.h for function documentation.
 */
error_type_t nvram_set_var(nvram_handle_t *nHandle, void *Variables)
{
    uint32_t           headerLen = sizeof(VARBLOCK_HEADER_t);
    error_type_t       status;
    VARBLOCK_HEADER_t *varBlock;
    NV_WRITE_Q_t      *NVData;
    int                px_status;
    uint32_t           dataLen;

    if ((nHandle == NULL) || (nHandle->BlockLabel[0] == 0) || (Variables == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    if (nHandle->MaxLength <= headerLen)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): invalid block size\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    
    /* Alloc memory so we can create a new block (header plus data) */   
    varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(nHandle->MaxLength);
    if (varBlock == NULL)
    {
        return FAIL;
    }

    /* We need to get the header values from an existing block, so read it from a
     * pending/active write or the physical part.  Note that we must hold the list
     * protection semaphore before using the list (and we can't put it until our
     * copy operation is done)!
     */
    px_status = pthread_mutex_lock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 
     
    /* Check the queued (not active write) first; it's the most recent data */
    NVData = FindWriteListEntry(nHandle->BlockLabel, NOT_ACTIVE_WR, &NVRAMWriteList);
    if (NVData == NULL)
    {
        /* Perhaps we have data actively writing to the device */
        NVData = FindWriteListEntry(nHandle->BlockLabel, ACTIVE_WR, &NVRAMWriteList);
    }
    
    if (NVData != NULL)
    {
        memcpy(varBlock, (int8_t *)NVData->DataBuff, headerLen);
    }
    
    /* Done with the queued write list, so unlock it */
    px_status = pthread_mutex_unlock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 
    
    /* If we didn't get the data from a pending write we need to go to the part */
    if (NVData == NULL)
    {
        if (VarBlockRead(nHandle, (int8_t *)varBlock, UINT64_C(0), headerLen) != headerLen)
        {
            MEM_FREE_AND_NULL(varBlock);
            return FAIL;
        }
    }

    /* We have the header, now copy the provided data into the block */
    dataLen = nHandle->MaxLength - headerLen;
    memcpy(varBlock->Data.Vars, Variables, dataLen);
    varBlock->Len = dataLen;

    /* Update the data checksum */
    varBlock->CRC = CalcChecksum( (uint32_t *)&varBlock->Data, sizeof(varBlock->Data) + varBlock->Len);

    
    /* Send the write to the background thread.  Note that it will make a copy
     * of the data so we can free ours.
     */
    status = NVMWrite(nHandle, (int8_t *)varBlock);

    MEM_FREE_AND_NULL(varBlock);
    return status;
}


/*
 * FUNCTION NAME: FlushNvram 
 *
 * Please see nvram_api.h for function documentation.
 */
error_type_t nvram_flush(void)
{
    int px_status;
    bool stay_in_loop = true;
    
    /*
     * While the write thread is busy processing the list it holds the
     * NVRAMWaitSem semaphore.  If we can get the semaphore then the write 
     * thread is idle.  There may be a possible race condition where we
     * could drop a write so check to make sure there are no writes waiting 
     * before returning from this routine.
     */

    /* Wait for the write thread to release the semaphore */
    while(stay_in_loop)
    {
        px_status = sem_wait( &nvram_wait_semaphore );
        ASSERT( px_status==0 );
		
        //
        // See if the write list is empty, if not release the semaphore and go back
        //
        if(ATIsListEmpty(&NVRAMWriteList))
        {
            stay_in_loop = false;
        }
    
        /* We got it, put it back */
        px_status = sem_post( &nvram_wait_semaphore );
        ASSERT( px_status==0 );
        posix_sleep_ticks(2); // let the write run if needed.
    }
        
    return OK;
}

void nvram_shutdown(void)
{
    nvram_flush();
}


error_type_t nvram_reset_all_vars(nvram_init_type_t reset_type)
{
    ATLISTENTRY      *searchNode;
    NV_INIT_FUNC_Q_t *myNode;
    VARBLOCK_HEADER_t *varBlock  = NULL;
    uint32_t i;
    error_type_t status;
    uint32_t InitLocationSize;
    int px_status;

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "Reset all vars type %x\n",reset_type);

    // hold off writes until this process is complete
    px_status = pthread_mutex_lock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 

    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    //
    // Get the first item off of the registered list.
    searchNode = ATListHead(&NVRAMInitFuncList);    // the list of the registered variable blocks.

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "search node %x # %x\n", searchNode, ATNumListNodes(&NVRAMInitFuncList)); 
    //
    // Now scan the entire list doing the resets.
    //
    for(i = 0; i < ATNumListNodes(&NVRAMInitFuncList); i++)
    {
        myNode = CONTAINING_RECORD(searchNode, NV_INIT_FUNC_Q_t, listNode);
        status = LocateVarBlock(myNode->blockHandle);
        ASSERT(OK == status);
        
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "variable id %.4s size 0x%x\n",myNode->ModuleLbl, 
                   myNode->blockHandle->MaxLength);
        varBlock = (VARBLOCK_HEADER_t *)MEM_MALLOC(myNode->blockHandle->MaxLength);  // get storage
        InitLocationSize = myNode->blockHandle->MaxLength - sizeof(VARBLOCK_HEADER_t);
        ASSERT(varBlock != NULL);

        // read the block in
        if (VarBlockRead(myNode->blockHandle, (int8_t *)varBlock, UINT64_C(0), myNode->blockHandle->MaxLength) != myNode->blockHandle->MaxLength)
        {
            // on an error, print the error and continue.
            //
            DBG_PRINTF(LOG_ERR|DBG_NVRAM,"get of var %.4s failed status 0x%x\n", myNode->ModuleLbl,
                       status);        
            searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
            continue;
        }

        // Now call the init function with the values needed.
        //
        status = (myNode->NvramInitFunction)(reset_type,  varBlock->Data.Version, 
                                             (void *)varBlock->Data.Vars, InitLocationSize,
                                             myNode->userData);
        if(status != OK)
        {
            // if the init failed, continue.
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "nvram init status failed %x\n", status);         
            searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
            continue;
        }
        //
        // Now write the variables back to the nvram part.
        //
        status = nvram_set_var(myNode->blockHandle, varBlock->Data.Vars);
        if(status != OK)
        {
            // on error notify and continue.
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "set var failed for %.4s status 0x$x\n",
                       myNode->ModuleLbl, status);

        }
        // return the allocated memory
        MEM_FREE_AND_NULL(varBlock);
        // get the next entry in the list.
        //
        searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "next node %x\n", searchNode); 
    }

    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    px_status = pthread_mutex_unlock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 

    // flushing the write queue isn't technically necessary, but since all variables were affected,
    // let's not return until the results are fully committed to the physical part
    status = nvram_flush();
    ASSERT(OK == status);

    return OK;
}

/**
 *
 *  \brief Queues a write to the background thread
 *
 *  Adds the current request to the write list and then sends an event to the
 *  write thread.
 *
 *  \warning We must have the list protection semaphore before working with
 *           the write list
 *
 *  \param[in] *nHandle Pointer to an open variable block handle
 *  \param[in] *Src     Pointer to the variable block data to write
 *
 *  \return error_type_t
 *  \retval OK    Write successfully queued
 *  \retval FAIL  Error queueing write
 *
 **/
static error_type_t NVMWrite(nvram_handle_t *nHandle, int8_t *Src)
{
    NV_WRITE_Q_t *myNode;
    NV_WRITE_Q_t *oldNode;
    int           px_status;
    
    if ((nHandle == NULL) || (nHandle->MaxLength == 0)  || (Src == NULL))
    {
        ASSERT(false);
        return FAIL;
    }
    
    /* Going to push the write to a background thread, allocate space for the
     * current write request (queue header + data length)
     */
    myNode = (NV_WRITE_Q_t *)MEM_MALLOC(sizeof(NV_WRITE_Q_t) + nHandle->MaxLength);
    if (myNode == NULL)
    {
        return FAIL;
    }

    /* Fill in the data header */
    memcpy(&myNode->Instance, nHandle, sizeof(nvram_handle_t));
    myNode->DataBuff    = (uint32_t *)((int8_t *)&myNode[0] + sizeof(NV_WRITE_Q_t));
    myNode->activeWrite = NOT_ACTIVE_WR;
    
    /* Copy the actual data buffer */
    memcpy(myNode->DataBuff, Src, nHandle->MaxLength);
    ATInitNode(&myNode->listNode);

    /* Get the access control lock and queue the write */
    px_status = pthread_mutex_lock( &nvram_global_lock ); 
    if (px_status == 0)
    {
        /* Search list to see if we have a pending write for the same block, if
         * so remove it before adding the newer version.  In this case we need to
         * ignore any actively writing blocks!
         */
        oldNode = FindWriteListEntry(nHandle->BlockLabel, NOT_ACTIVE_WR, &NVRAMWriteList);
        if (oldNode != NULL)
        {
            DBG_PRINTF(LOG_NOTICE|DBG_NVRAM, "NVRAMAPI(%d): New write replacing queued write for '%4s'\n", 
                      __LINE__, nHandle->BlockLabel);
            ATRemoveEntryList(&oldNode->listNode);
            MEM_FREE_AND_NULL(oldNode);
        }
        ATInsertTailList(&NVRAMWriteList, &myNode->listNode);
        
        /* Release the list protection lock */
        px_status = pthread_mutex_unlock( &nvram_global_lock ); 
        XASSERT(px_status == 0, px_status); 
    }
    else
    {
        /* We failed to get the semaphore */
        MEM_FREE_AND_NULL(myNode);
        return FAIL;
    }

    /* Kick the write thread to get things going */
        px_status = sem_post( &nvram_event_semaphore );
        ASSERT( px_status==0 );
        
    return OK;
}


/**
 *
 *  \brief Thread to write NVRAM variables to physical memory
 *
 *  This thread processes background write requests.
 **/
void *NVRAMWriteThread(void *unused)
{
    NV_WRITE_Q_t   *dataEntry;
    nvram_handle_t    nHandle;
    int             px_status;

    /* The background write thread should never exit */
    while (1)
    {

        /* Wait for an event */
        px_status = sem_wait( &nvram_event_semaphore );
        ASSERT( px_status==0 );
        
        /* Get the semaphore to block the NvramFlush() api until all data written
         * It adds overhead to NVRAM write thread as it may have to wait on semaphore
         * but is required to block NvramFlush() api
         */
        px_status = sem_wait( &nvram_wait_semaphore );
        ASSERT( px_status==0 );

        /* Mark the head as active and return a pointer.  makeActiveWriteEntry will
         * use the list protection semaphore.
         */
        while ((dataEntry = makeActiveWriteEntry()) != NULL)
        {
            /* We should have a pointer to the active head node */
            ASSERT(dataEntry->activeWrite == ACTIVE_WR);
            
            /* Copy in the cached nvram instance structure */
            memcpy(&nHandle, &dataEntry->Instance, sizeof(nvram_handle_t));    
            
            /* Make the write call (may block at device driver level) */
            if (VarBlockWrite(&nHandle, (int8_t *)dataEntry->DataBuff) != nHandle.MaxLength)
            {
                DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Background NVRAM write failed for '%4s'\n", 
                             dataEntry->Instance.BlockLabel);
            }
            else
            {
                /*
                DBG_PRINTF(LOG_NOTICE|DBG_NVRAM, "Background NVRAM write completed for '%4s'\n", 
                             dataEntry->Instance.BlockLabel);
                 */
            }

            /* Pop the active entry from list and free it (the get routine will get
             * the semaphore will working the list) 
             */
            dataEntry = popActiveWriteEntry();
            if (dataEntry == NULL)
            {
                /* Our active entry went missing, not good */
                ASSERT(false);
            }
            else
            {
                MEM_FREE_AND_NULL(dataEntry);
            }
        }
        
        /* Unblock the flush api routine */
        px_status = sem_post( &nvram_wait_semaphore );
        ASSERT( px_status==0 );
    }
    
    return 0;
}


/**
 *  \brief Makes the entry on the head of list active
 *
 *  This routine marks the head node on the queued write list as active
 *  and returns a pointer to the caller.  We must hold the list protection
 *  semaphore before working with the list.  
 *
 *  Only the head of the list should be marked as active write.
 *
 *  \return *NV_WRITE_Q_t  pointer to active write entry
 *  \retval Write list entry pointer on success
 *  \retval NULL on failure
 *
 */
static NV_WRITE_Q_t *makeActiveWriteEntry(void)
{
    NV_WRITE_Q_t  *dataEntry = NULL;
    ATLISTENTRY   *node;
    int px_status;

    /* We must get the lock before working the list */
    px_status = pthread_mutex_lock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 

    /* Grab the head right off the list */
    node = ATRemoveHeadList(&NVRAMWriteList);
    if (node != NULL)
    {
        dataEntry = CONTAINING_RECORD(node, NV_WRITE_Q_t, listNode);
        if (dataEntry != NULL)
        {
            /* Mark entry as active */
            dataEntry->activeWrite = ACTIVE_WR;
        }
        else
        {
            /* This would be strange */
            ASSERT(false);
        }
                
        /* Put the node back on the list */
        ATInsertHeadList(&NVRAMWriteList, node);
    }

    /* Release the lock */
    px_status = pthread_mutex_unlock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 

    return dataEntry;
}


/**
 *  \brief Removes the entry from head of queued write list
 *
 *  This routine removes the head node from the queued write list and returns
 *  it to the caller.  We must hold the list protection semaphore before 
 *  working with the list.  The head of the list should be marked as active
 *  write.
 *
 *  \return *NV_WRITE_Q_t  pointer to queued write entry
 *  \retval Write list entry pointer on success
 *  \retval NULL on failure
 *
 */
static NV_WRITE_Q_t *popActiveWriteEntry(void)
{
    NV_WRITE_Q_t  *dataEntry = NULL;
    ATLISTENTRY   *node;
    int px_status;

    /* We must hold the lock before working the list */
    px_status = pthread_mutex_lock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 

    /* The active write node should always be on the head of list */
    node = ATRemoveHeadList(&NVRAMWriteList);

    if (node != NULL)
    {
        dataEntry = CONTAINING_RECORD(node, NV_WRITE_Q_t, listNode);
        if (dataEntry != NULL)
        {
            ASSERT(dataEntry->activeWrite == ACTIVE_WR);
            /*
            DBG_PRINTF(LOG_NOTICE|DBG_NVRAM, "Removed active node '%4s' from write queue\n", 
                          dataEntry->Instance.BlockLabel);
             */
        }
    }

    /* Release the lock */
    px_status = pthread_mutex_unlock( &nvram_global_lock ); 
    XASSERT(px_status == 0, px_status); 
    
    return dataEntry;
}


/**
 *  \brief Searches the queued write list
 *
 *  Note that we are searching for both the block label and the active write flag
 *
 *  \warning The caller must hold the NV write list protection semaphore before
 *           calling this routine.
 *
 *  \param[in] *BlockLabel Pointer to a 4 character variable block label
 *  \param[in] activeFlag  active write flag
 *  \param[in] *List       Pointer to the queued write list
 *
 *  \return *NV_WRITE_Q_t
 *  \retval  pointer to NV write list entry if found
 *  \retval  NULL if specified entry not found (or error)
 */
static NV_WRITE_Q_t *FindWriteListEntry(char *BlockLabel, bool activeFlag, ATLISTENTRY *List)
{
    ATLISTENTRY   *searchNode;
    NV_WRITE_Q_t  *myNode;

    if ((List == NULL) || (BlockLabel == NULL))
    {
        return NULL;
    }
    
    /* Set searchnode to first entry after the start */
    searchNode = List->nextEntry;
    if (searchNode->nextEntry == searchNode)
    {
        return NULL;
    }

    while (searchNode != List)
    {
        myNode = CONTAINING_RECORD(searchNode, NV_WRITE_Q_t, listNode);
        if ( (myNode != NULL) && (myNode->activeWrite == activeFlag) && 
             (memcmp(&myNode->Instance.BlockLabel[0], BlockLabel, 4) == 0) )
        {
            return myNode;
        }
        searchNode = searchNode->nextEntry;
    }
    
    return NULL;
}


/**
 * \brief Finds NvramInitFunction entry with matching ModuleID
 *
 * \param[in] *ModuleLbl  Pointer to a 4 character variable block label
 * \param[in] *FuncList   Pointer to the init function list
 *
 * \return *NV_INIT_FUNC_Q_t
 * \retval  pointer to init function list entry if found
 * \retval  NULL if specified entry not found (or error)
 *
 */
static NV_INIT_FUNC_Q_t *FindInitFuncEntry(char *ModuleLbl, ATLISTENTRY *FuncList)
{
    ATLISTENTRY      *searchNode;
    NV_INIT_FUNC_Q_t *myNode;

    if ((FuncList == NULL) || (ModuleLbl == NULL))
    {
        return NULL;
    }

    /* Set searchnode to first entry after the start */
    searchNode = FuncList->nextEntry;
    if (searchNode->nextEntry == searchNode)
    {
        return NULL;
    }

    while (searchNode != FuncList)
    {
        myNode = CONTAINING_RECORD(searchNode, NV_INIT_FUNC_Q_t, listNode);
        if ((myNode != NULL) && (memcmp(&myNode->ModuleLbl[0], ModuleLbl, 4) == 0))
        {
            return myNode;
        }
        searchNode = searchNode->nextEntry;
    }
    
    return NULL;
}


/**
 * \brief Stores the NvramInitFunction pointer for specified module/block
 *
 * \param[in] *ModuleLbl         Pointer to 4 character module/block label
 * \param[in] *NvramInitFunction Pointer to Nv init function
 *
 **/
static void SetNvInitFuncPtr(char *ModuleLbl, nvram_init_func_ptr NvramInitFunction, void *userData, nvram_handle_t *handle)
{
    NV_INIT_FUNC_Q_t *oldNode;
    NV_INIT_FUNC_Q_t *newNode;
    int px_status;

    if ((ModuleLbl == NULL) || (NvramInitFunction == NULL))
    {
        ASSERT(false);
        return;
    }

    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    /* Look to see if we already have an init function registered */
    oldNode = FindInitFuncEntry(ModuleLbl, &NVRAMInitFuncList);
    if (oldNode != NULL)
    {
        /* We already have a node, simply update it */
        oldNode->NvramInitFunction = (nvram_init_func_ptr) NvramInitFunction;
        oldNode->userData = userData;
        oldNode->blockHandle = handle;
    }
    else
    {
        /* Looks like we need to create a new node */
        newNode = (NV_INIT_FUNC_Q_t *)MEM_MALLOC(sizeof(NV_INIT_FUNC_Q_t));
        ASSERT(newNode != NULL);

        /* Initialize the new node */
        memcpy(&newNode->ModuleLbl[0], ModuleLbl, 4);
        newNode->NvramInitFunction = NvramInitFunction;
        newNode->userData = userData;
        newNode->blockHandle = handle;
       
        /* Add our new node to the list*/
        ATInsertTailList(&NVRAMInitFuncList, &newNode->listNode);
    }

    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );
}

#ifdef HAVE_CMD
static int vars_unregister_proc(int argc, char *argv[])
{

    ATLISTENTRY      *searchNode;
    NV_INIT_FUNC_Q_t *myNode;
    uint32_t device_id;
    uint32_t i;
    uint32_t str_len;
    char my_id[4];
    int px_status;

    if(argc != 2 || strlen(argv[1]) > 4)
    {
        return CMD_USAGE_ERROR;
    }
    str_len = strlen(argv[1]);
    strncpy(my_id, argv[1], str_len);
    // pad the end with spaces if needed.
    // each id is 4 bytes, if our input label is < 4 bytes pad with
    // spaces
    //
    while(str_len < 4)
    {
        my_id[str_len++] = 0x20;    // put in a space
    }
    // Now get the result into a uint32
    device_id = ((uint32_t *)&my_id)[0];

    //
    // Now search through the list for the requested handle.
    //
    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    searchNode = ATListHead(&NVRAMInitFuncList);    // the list of the registered variable blocks.
    for(i = 0; i < ATNumListNodes(&NVRAMInitFuncList); i++)
    {
        myNode = CONTAINING_RECORD(searchNode, NV_INIT_FUNC_Q_t, listNode);
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "test id %x, cur_id %x\n", device_id,
                   ((uint32_t *)myNode->blockHandle->BlockLabel)[0]); 
        if(((uint32_t *)myNode->blockHandle->BlockLabel)[0] == device_id)
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM, "Found label %x, remove it\n", device_id); 
            nvram_variable_unregister(myNode->blockHandle);

            px_status = sem_post( &nvram_init_funclist_semaphore );
            ASSERT( px_status==0 );

            return OK;  // removed, get out.
        }
        searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
    }
    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    return CMD_ERROR;
}
/**
 * \brief List all the registered var blocks
 */
static int vars_list_proc(int argc, char *argv[])
{
    ATLISTENTRY *searchNode;
    NV_INIT_FUNC_Q_t *myNode;
    uint32_t i;
    uint32_t size;
    int px_status;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    
    size = 0;

    px_status = sem_wait( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );

    searchNode = ATListHead(&NVRAMInitFuncList);    // the list of the registered variable blocks.
    cmd_printf( "Block ID\tSize\tDevice ID\n---------------------------------\n" );
    for(i = 0; i < ATNumListNodes(&NVRAMInitFuncList); i++)
    {
        myNode = CONTAINING_RECORD(searchNode, NV_INIT_FUNC_Q_t, listNode);
        
        cmd_printf( "%.4s\t\t0x%x\t0x%x\n",
                    myNode->ModuleLbl, 
                    (unsigned int)myNode->blockHandle->MaxLength,
#ifdef HAVE_NVRAM_FILE
                    (unsigned int)myNode->blockHandle->fd );
#else
                    (unsigned int)myNode->blockHandle->DeviceID );
#endif                    
        size += myNode->blockHandle->MaxLength;
        searchNode = ATListNext(&NVRAMInitFuncList, searchNode);
    }
    cmd_printf( "\nTotal var size %d bytes\n", (int)size );

    px_status = sem_post( &nvram_init_funclist_semaphore );
    ASSERT( px_status==0 );
    
    return CMD_OK;
}

static int nvram_reset_proc(int argc, char* argv[])
{
    nvram_init_type_t value;

    if( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    // check the input and set the parameter variable.
    if(strncasecmp("FACTORY", argv[1], 7) == 0)
    {
        value = NVRAM_FACTORY_DEFAULTS;
    }
    else if(strncasecmp("USER",argv[1], 4) == 0)
    {
        value = NVRAM_USER_DEFAULTS;
    }
    else if(strncasecmp("LANG", argv[1], 4) == 0)
    {
        value = NVRAM_LANG_DEFAULTS;
    } 
#ifdef DEBUG
    else if(strncasecmp("NEW", argv[1], 3) == 0 )
    {
        value = NVRAM_NEW;
    }
    else if(strncasecmp("ERROR", argv[1], 5) == 0 )
    {
        value = NVRAM_ERROR;
    }
    else if(strncasecmp("VERSION", argv[1], 7) == 0)
    {
        value = NVRAM_VERSION_CHANGE;
    }
#endif
    else 
    {
        // didn't find anything return error.
        return CMD_USAGE_ERROR;
    }

    cmd_printf( "nvram init type: value %x\n", value );
    // reset all the variables.
    nvram_reset_all_vars(value);

    return CMD_OK;
}

static void nvram_register_cmds(void)
{
    int cmd_res;
    cmd_res = cmd_register_cmd( "nvram",
                                 NULL,
                                 "Commands for nvram",
                                 NULL, 
                                 NULL,
                                 NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "nvram",
                                   "reset",
                                   "Global reset of variables",
                                   "<reset value>", 
                                   "reset value is one of: 'LANG' -- language reset, 'FACTORY' -- factory reset, 'USER' -- user reset, "
                                     "'NEW' -- simulate new block (debug only), 'ERROR' -- simulate block error (debug only), 'VERSION' -- simulate version change (debug only)",
                                   nvram_reset_proc );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "nvram",
                                   "list",
                                   "List all var blocks and lengths",
                                   NULL,
                                   NULL,
                                   vars_list_proc );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "nvram",
                                   "unregister",
                                   "Un-register a var block",
                                   "<BLOCK_ID>",
                                   "<BLOCK_ID> is case sensitive",
                                   vars_unregister_proc );

}
#endif /* HAVE_CMD */

