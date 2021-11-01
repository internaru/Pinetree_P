/**
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_dev_NAND.c
 *
 *  \brief NVRAM NAND device shim layer
 *
 *  This file manages blocks of NVRAM variables on NAND devices. The variable
 *  block storage algorithm is designed specifically to do wear leveling on
 *  NAND devices.
 *  We will use 2 blocks. Page 0 of a block will have a 'base
 *  image' of the NV.  The remaining pages of the block will
 *  have 'addendums'.
 *  
 *  Once 'addendums' have a need to exceed the size of block,
 *  the 'backup block' will be erased - the addendums are folded
 *  into a new 'base image' and written into the new active
 *  block.
 *  
 *  The old 'active block' is now backup, it is not erased, it
 *  has a 'time stamp' such that at power up, we will decide the
 *  'active block' by the block with the largest timestamp.
 *  
 *  Do crc already exist?
 *  Page 0 probably needs a header: ver, size baseImage, [crc]
 *  Does each addendum have header: ver, size baseImage, [crc]
 *
 *  Container Partition -->   +------------------------+
 *  Start                     | - Block Signature      |    (Active Block = 0)
 *                            | - 'PRNT' Vars          |
 *                            | - 'SCAN' Vars          |
 *                            | - 'PLAT' Vars          |
 *                            | - empty - end baseImage|
 *                            |---next page -----------|
 *                            | - 'SCAN' Vars(addendum)|
 *                            |---next page -----------|
 *      DyWriteOffsetNAND --> |    (Erased Space)      |
 *                            |                        |
 *                            |                        |
 *                       +--> +------------------------+
 *                       |    |  previous baseImage    |    (Backup Block = 1)
 *                       |    |                        |
 *    MaxDyPartionLen ---|    |                        |
 *                       |    |  and addendums....     |
 *                       |    |                        |
 *                       |    |                        |
 *  Container End     ---+--> +------------------------+
 *
 *
 *
 *  \warning
 *  The current implementation only supports managing a container partition on
 *  a single memory device: we count on the dev shim always calling with the same
 *  memDev.  Support for multiple devices is possible (by putting the static
 *  state variables in a list of structures), but is not currently a requirement.
 *
 *  Note that we are running in a multi-threaded environment so we need to be a 
 *  little careful with our internal state variables.  On the top the NVM Manager 
 *  API will do some things to help us:
 *   - serialize write operations
 *   - read operations will take data from a pending write queue before calling 
 *     us to read the part
 *  And on the bottom the physical device drivers will only allow one operation at
 *  a time (for a given memory device).  
 *
 *  So at the end of the day it's probably safest to make all our operations in
 *  this file atomic (it will end up that way when we hit the device anyways ... 
 *  because we only support one physical memory device for the NV store).
 *
 **/

#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "posix_ostools.h"

#include "error_types.h"
#include "memAPI.h"
#include "dprintf.h"
#include "logger.h"
#include "list.h"
#include "lassert.h"
#include "nodeCache.h"
#include "deviceid.h"
#include "partitionMgrAPI.h"
#include "nvram_api.h"
#include "nvram_dev_shim.h"
#include "nvram_dev_nand.h"
#include "nvram_logger.h"
#include "ioutils.h"
#include "utils.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifdef DEBUG
    #include "debug.h"
#endif

#ifdef HAVE_DBG_PRINTF
  #ifndef DBGOPT
    #define DBGOPT (DBG_LOUD | DBG_TERMINAL)
  #endif  
#endif

#define NVNAND_DBG_LVL (LOG_INFO)

/*
 * Each 'dynamic' block record in NAND has a header three 32-bit words long:
 *    +-----------+------------+----------+----------------------------
 *    |    ID     |   LENGTH   |  OFFSET  |  Length bytes of Data ...
 *    |    (0)    |    (1)     |   (2)    |
 *    +-----------+------------+----------+----------------------------
 */
#define NAND_ID_OFFSET          0           /**< Offset in bytes to ID field       */
#define NAND_LEN_OFFSET         4           /**< Offset in bytes to Length field   */
#define NAND_OFFSET_OFFSET      8           /**< Offset in bytes to offset field   */
#define NAND_HDR_LEN            12          /**< Header length in bytes            */

#define UNUSED_DATA32          0xffffffff  /**< Unused/erased NAND data            */
#define UNUSED_DATA8           0xff        /**< Unused/erased NAND data            */

#define MIN_NAND_PARTSIZE       0x20000     /**< Smallest partition size, in bytes */

#define NAND_VAR_BLOCK_SIG      0xFACE      /**< Container partition signature     */
#define NAND_VAR_BLOCK_SIG_SOLO 0xF0A0C0E0  /**< Container partition signature - alone on page, data in subsequent pages */

/*
 * Local variable types
 */

typedef struct                     /* Note: sizeof(DY_HDR_t) = NAND_HEADER_LEN     */
{
    char      BlockLbl[4];         /**< Variable block label (4 chars, 32 bits)   */
    uint32_t  MaxLength;           /**< Maximum length of variable block          */
    uint32_t  StartOffset;         /**< Offset from the start of active partition */
} DY_HDR_t;


/*
 * Static variable declarations
 */
static pthread_mutex_t mutex_nvramNAND;
static uint32_t      DyWriteOffsetNAND = 0;      /* Points to next write offset in NAND memory */
static uint32_t      DyNandPageSize    = 0;      /* Use to calculate next write location  */
static ATLISTENTRY  *VarBlockListNAND = NULL;    /* List of variable blocks on NAND            */
static uint32_t      MaxDyPartionLen = 0;       /* Maximum length of dynamic partition       */
static uint32_t      ActiveBlock     = 0;       /* Active data block in container partition  */
static bool          NANDInitComplete = false;

/*
 * Static function declarations
 */
static error_type_t setDyWritePtrNAND(uint32_t, bool);
static uint32_t     getDyWritePtrNAND(void);
static void         setDyPartitionLength(uint32_t);
static uint32_t     getDyPartitionLength(void);
static error_type_t SwitchToBackupBlock(MEMORY_DEVICE memDev);
static uint32_t     GetCommittedSizeNAND(ATLISTENTRY *theList);
static void         setDyPageSize(uint32_t);
static uint32_t     getDyPageSize(void);

#ifdef HAVE_CMD
  static void ListBlocks(ATLISTENTRY *blkList);
  static int  NVNAND_callback(int argc, char *argv[]);
#endif


/*
 * FUNCTION NAME: InitNvramNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
void InitNvramNAND(void)
{
    int px_status;
#ifdef HAVE_CMD
    int i_res;
#endif

    /* Make sure our constants match the actual data types */
    ASSERT(NAND_HDR_LEN == sizeof(DY_HDR_t));

    /* Only run through init code once */
    if (NANDInitComplete == false)
    {
        NANDInitComplete = true;

        VarBlockListNAND = (ATLISTENTRY *)MEM_MALLOC(sizeof(ATLISTENTRY));
        ASSERT(VarBlockListNAND != NULL);

        /* Initialize variable block list */
        ATInitList(VarBlockListNAND);

        /* Create mutex for protecting the NAND write pointer */
        px_status = posix_mutex_init( &mutex_nvramNAND );
        XASSERT( px_status==0, px_status );
#ifdef HAVE_CMD
        i_res = cmd_register( "nvnand",
                              NULL,
                              NULL,
                              NULL,
                              NVNAND_callback );
        // ASSERT( OK == e_res );
#endif
    }
}


/*
 * FUNCTION NAME: GetPartitionNeedsNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
error_type_t GetPartitionNeedsNAND(MEMORY_DEVICE      memDev, 
                                  uint32_t          *partitionSize)
{
    uint32_t   NANDBlockCount;
    uint32_t   NANDBlockSize;
    uint32_t   NANDPageSize;
    uint32_t   neededBlocks;

    /* Note: no state variables used in this API routine, so mutex protection
     *       is not required.
     */
             
    /* Get the NAND part block size */
    if (PARTMGR_OK != GetDeviceInfo(memDev, &NANDBlockCount, &NANDBlockSize, &NANDPageSize))
    {
        ASSERT(false);
        return FAIL;
    }

    setDyPageSize(NANDPageSize);

    /* Calculate the needed number of blocks to meet the minimum container
     * partition size 
     */    
    neededBlocks = MIN_NAND_PARTSIZE / NANDBlockSize;
    if (MIN_NAND_PARTSIZE % NANDBlockSize)
    {
        /* Round up if the requested size falls into next block (or fell short) */
        neededBlocks += 1;
    }

    /* Double the space for our wear leveling algorithm (active/backup areas) */
    neededBlocks *= 2;

    /* Set return values */
    *partitionSize = neededBlocks * NANDBlockSize;

    return OK;
}


/*
 * FUNCTION NAME: GetVarBlockSizeNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
uint32_t GetVarBlockSizeNAND(MEMORY_DEVICE memDev, char *label)
{
    int px_status;
    BLOCK_NODE_t *myNode;
    uint32_t      retSize = 0;
 
    /* Acquire mutex */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }
   
    /* Look for the var block, set size if found */
    myNode = FindNodeByLabelAndDev(label, memDev, VarBlockListNAND);
    if (myNode != NULL)
    {
        retSize = myNode->Size;
    }
 
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );

    return retSize;
}


/*
 * FUNCTION NAME: VarBlockReadNAND
 *
 *   See nvram_dev_NAND.h for function documentation.
 */
uint32_t VarBlockReadNAND(nvram_handle_t   *nHandle, 
                         int8_t         *blockPtr,
                         uint64_t        offset, 
                         uint32_t        size)
{
    int px_status;
    BLOCK_NODE_t       *partEntry;
    PARTMGR_ERROR_t     partErr;
    uint64_t            activeBlockOffset;
    uint64_t            readAddr;
    uint32_t            len     = 0;
    partition_handle_t *pHandle = NULL;
    
    /* Validate input parameters */
    if ((nHandle == NULL) || (size == 0) || (blockPtr == NULL))
    {
        ASSERT(false);
        return 0;
    }

    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }
    
    /* Find the entry in the variable block list */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, VarBlockListNAND);

    /* Can't read data if it doesn't exist or hasn't been committed (written) */
    if ((partEntry == NULL) || (partEntry->Committed == false))
    {
        goto EXIT;
    }

    /* Make sure we aren't reading off end of our var block */
    if ((offset + size) > partEntry->Size)
    {
        DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): attempt to read past end of var block\n", __LINE__);
        goto EXIT;
    }
    
    /* Open our container partition */
    partErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, nHandle->DeviceID, &pHandle);
    if (partErr != PARTMGR_OK)
    {
       goto EXIT; 
    }

    /* Read in the variable block */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();
    readAddr = activeBlockOffset + partEntry->StartOffset + offset;
    partErr = ReadPartition(pHandle, blockPtr, readAddr, size, &len);
    if ((partErr != PARTMGR_OK) || (len != size))
    {
        /* Device read failure.  Set len to 0 to help indicate failure */
        DBG_PRINTF(LOG_INFO,"NV_DEV_NAND(%d): device read failure\n", __LINE__);
        len = 0;
        goto EXIT;
    }

EXIT:
    /* Make sure we close the partition if opened */
    if (pHandle != NULL)
    {
        ClosePartition(pHandle);
    }
    
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );
       
    return len;
}


/*
 * FUNCTION NAME: VarBlockWriteNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
uint32_t VarBlockWriteNAND(nvram_handle_t *nHandle, int8_t *Src)
{
    int px_status;
    uint32_t            writeOffset;
    uint32_t            writeLen;
    DY_HDR_t            dyHeader;
    BLOCK_NODE_t       *partEntry;
    PARTMGR_ERROR_t     partErr;
    uint64_t            activeBlockOffset;
    uint32_t            retLen   = 0;
    int8_t             *dataBuff = NULL;
    partition_handle_t *pHandle  = NULL;
    
    /* Validate input parameters */
    if ((nHandle == NULL) || (nHandle->MaxLength == 0) || (Src == NULL))
    {
        ASSERT(false);
        return 0;
    }
    
    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }

    /* Get the NAND write pointer (next write address). It will be word aligned. */
    writeOffset = getDyWritePtrNAND();
    ASSERT(writeOffset != 0);
    
    /* Get the actual write length (block size plus our NAND header) */
    writeLen = nHandle->MaxLength + sizeof(DY_HDR_t);
    
    /* Check if write needs to move to next container of partition space
     * 1 - all pages are full (occupied - won't fill each page, page can take only 1 write and keep a valid ECC)
     * 2 - empty page exists, but 'write size' is > space available 
     */ 
    if ((writeOffset + writeLen) > getDyPartitionLength())
    {
        /* We hit the end of the active block.  Because our wear leveling
         * creates multiple copies of var blocks as they are written, moving to
         * the backup block will probably reclaim space.
         */
        if (SwitchToBackupBlock(nHandle->DeviceID) != OK)
        {
            /* The swap ran into trouble, not good */
            goto EXIT;
        }

        /* Get the updated write pointer and see if we have space now */
        writeOffset = getDyWritePtrNAND();
        if ((writeOffset + writeLen) > getDyPartitionLength())
        {
            /* Our partition is full, need to create a bigger container */
            ASSERT(false);
            goto EXIT;
        }
    }

    /* Create a header that reflects the actual target offset */
    memcpy(&dyHeader.BlockLbl[0], &nHandle->BlockLabel[0], 4);
    dyHeader.StartOffset = cpu_to_be32( writeOffset + sizeof(DY_HDR_t) );
    dyHeader.MaxLength   = cpu_to_be32( nHandle->MaxLength );
  
    /* Allocate a buffer for the var block data and header */
    dataBuff = (int8_t *)MEM_MALLOC(writeLen);
    if (dataBuff == NULL)
    {
         DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): unable to allocate memory for data buffer\n", __LINE__);
         goto EXIT;
    }

    /* Move header and data into our RAM buffer */
    memcpy(dataBuff, (int8_t *)&dyHeader, sizeof(DY_HDR_t));
    memcpy((dataBuff + sizeof(DY_HDR_t)), Src, nHandle->MaxLength);

    /* Open our container partition */
    partErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, nHandle->DeviceID, &pHandle);
    if (partErr != PARTMGR_OK)
    {
       goto EXIT; 
    }
    
    /* Write the data to the physical memory part */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();
    partErr = WritePartition(pHandle, (int8_t *)dataBuff, activeBlockOffset + writeOffset, writeLen, &retLen);
    if ((partErr != PARTMGR_OK) || (retLen != writeLen))
    {
        /* Device write failure.  Set len to 0 to help indicate failure 
         * REVISIT: where should the write pointer be left in this case?
         */
        DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): device write failure\n", __LINE__);
        retLen = 0;
        goto EXIT; 
    }
    else
    {
        /* Device write passed, but the caller can't know about our header length.
         * Fix up return size.
         */
        retLen = writeLen - sizeof(DY_HDR_t);
    }

    /* The block is written, update the write pointer */
    setDyWritePtrNAND(writeOffset + writeLen, true);
    
    /* Now our block is committed to the NV store, find the entry in the var block
     * list and modify to reference new offset
     */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, VarBlockListNAND);
    if (partEntry == NULL)
    {
        /* Should never happen: implies the block was unexpectedly deleted. Create
         * a new node and fill out the basic info.
         */
        partEntry = AllocNewNode();
        if (partEntry == NULL)
        {
            ASSERT(false);
            goto EXIT;
        }
        memcpy(&partEntry->Label[0], &dyHeader.BlockLbl[0], 4);
        partEntry->MemDev = nHandle->DeviceID;
    }
    else
    {
        /* Remove the existing block from the list (for updates) */
        ATRemoveEntryList(&partEntry->listNode);
    }

    /* Update the partEntry */
    partEntry->StartOffset = be32_to_cpu(dyHeader.StartOffset);
    partEntry->Size        = be32_to_cpu(dyHeader.MaxLength);
    partEntry->Committed   = true;

    /* Insert entry in block list (at tail to keep list ordered) */
    ATInsertTailList(VarBlockListNAND, &partEntry->listNode);
        
EXIT:
    /* Make sure we close the partition if opened */
    if (pHandle != NULL)
    {
        ClosePartition(pHandle);
    }

    if (dataBuff != NULL)
    {
        MEM_FREE_AND_NULL(dataBuff);
    }

    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );
       
    return retLen;
}


/*
 * FUNCTION NAME: RegisterVarBlockNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
error_type_t RegisterVarBlockNAND(char         *label, 
                                 MEMORY_DEVICE memDev, 
                                 uint32_t      maxSize)
{
    int px_status;
    BLOCK_NODE_t  *newEntry;
    BLOCK_NODE_t  *oldEntry;
    error_type_t   retCode = FAIL;

    /* Verify passed parameters are sane */    
    if ((label == NULL) || (maxSize == 0))
    {
        DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d):invalid label or size\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Test to see if the var block already exists */
    oldEntry = FindNodeByLabelAndDev(label, memDev, VarBlockListNAND);
    if (NULL != oldEntry)
    {
        /* Already exists, but only return OK if sizes match */
        if (oldEntry->Size == maxSize)
        {
            retCode = OK;
        }
        goto EXIT;
    }
     
    /* Allocate memory for the new var block entry and fill it out.
     * Note: updating the starting offset is a bit silly right now.  We don't
     * commit to an actual offset until the var block is written to the 
     * physical memory part.  Mark the block as non-committed.
     */
    newEntry = AllocNewNode();
    if (newEntry == NULL)
    {
        goto EXIT;
    }    
    memcpy(&newEntry->Label[0], label, 4);
    newEntry->Size      = maxSize;
    newEntry->MemDev    = memDev;
    newEntry->Committed = false;
     
    /* Insert in var block list */
    ATInsertTailList(VarBlockListNAND, &newEntry->listNode);
    
    retCode = OK;
    
EXIT:
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );
       
    return retCode;
}


/*
 * FUNCTION NAME: ReleaseVarBlockNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
error_type_t ReleaseVarBlockNAND(char         *label, 
                                MEMORY_DEVICE memDev)
{
    int px_status;
    BLOCK_NODE_t *partEntry;
    error_type_t  retStatus = FAIL;

    /* Acquire mutex */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Attempt to locate the block: remove it from the list if found */
    partEntry = FindNodeByLabelAndDev(label, memDev, VarBlockListNAND);
    if (partEntry != NULL)
    {
        ATRemoveEntryList(&partEntry->listNode);
        MEM_FREE_AND_NULL(partEntry);

        /* We can't simply remove the block from NAND memory (the entire NAND sector must
         * be erased).  Force a switch to the backup block which will not copy the 
         * removed block.
         */
        retStatus = SwitchToBackupBlock(memDev);
    }

    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );

    return retStatus;
}


/*
 * FUNCTION NAME: DiscoverVarBlocksNAND
 *
 * See nvram_dev_NAND.h for function documentation.
 */
error_type_t DiscoverVarBlocksNAND(MEMORY_DEVICE memDev)
{
    error_type_t        retStatus = FAIL;
    int px_status;
    uint32_t            partSig;
    DY_HDR_t            dyHeader;
    BLOCK_NODE_t       *newPartEntry;
    uint64_t            activeBlockOffset;
    uint32_t            NANDBlockCount;
    uint32_t            NANDBlockSize;
    uint32_t            NANDPageSize;
    PARTMGR_ERROR_t     partMgrErr;
    partition_info_t    Info;
    uint32_t            len;
    uint32_t            containSize;
    partition_handle_t *handle    = NULL;
    VARBLOCK_HEADER_t  *blockPtr  = NULL;
    uint32_t            bytesScan = sizeof(uint32_t);
    bool                bPageAlign = false;

    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramNAND );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }
    
    DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): Discover NV blocks on %s\n", __LINE__, getDevIdStr(memDev));
    
    /* Open our 'container' partition */
    if (PARTMGR_OK != OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &handle))
    {
        DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): NV container partition not present on %s\n",
                      __LINE__, getDevIdStr(memDev));
        goto EXIT;
    }

    /* Get the partition information */
    if (PARTMGR_OK != GetPartitionInfoByHandle(handle, &Info))
    {
        ASSERT(false);
        goto EXIT;
    }

    /* Get the NAND part block size */
    if (PARTMGR_OK != GetDeviceInfo(memDev, &NANDBlockCount, &NANDBlockSize, &NANDPageSize))
    {
        ASSERT(false);
        goto EXIT;
    }
    
    setDyPageSize(NANDPageSize);

    /* While we should have validated the container before now, going to double
     * check we are at least aligned properly.  Note we divide the container 
     * size by 2 to get active/backup sizes (the actual usable size).
     */
    containSize = Info.Size / 2;
    if (containSize % NANDBlockSize)
    {
        /* We should be block boundary aligned ... */
        DBG_PRINTF(LOG_ERR, "NV_DEV_NAND(%d): container not properly aligned\n", __LINE__);
        ASSERT(false);
        goto EXIT;
    }

    /* Update container partition length */
    setDyPartitionLength(containSize);

    /* Our variable block list should be empty at this point, but make sure */
    DumpNodeList(VarBlockListNAND);

    /* We need to find the active block. Read in the first 4 bytes of block 0
     * to for look for the block signature 
     */
    activeBlockOffset = UINT64_C(0);
    partMgrErr = ReadPartition(handle, (int8_t *)&partSig, activeBlockOffset, bytesScan, &len);
    if ((partMgrErr != PARTMGR_OK) || (len != bytesScan))
    {
        DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): device read failure\n", __LINE__);
        goto EXIT;
    }

    if (be32_to_cpu(partSig) == NAND_VAR_BLOCK_SIG_SOLO)
    {
        DBG_PRINTF(NVNAND_DBG_LVL, "Found Solo signature on page - adjust\n");
        bytesScan = getDyPageSize();
        bPageAlign = true;
    }
    else if (be32_to_cpu(partSig) != NAND_VAR_BLOCK_SIG)
    {
        /* No signature in block 0, read the next container boundary */
        activeBlockOffset = getDyPartitionLength();
        partMgrErr = ReadPartition(handle, (int8_t *)&partSig, activeBlockOffset, bytesScan, &len);
        if ((partMgrErr != PARTMGR_OK) || (len != bytesScan))
        {
            DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): device read failure\n", __LINE__);
            goto EXIT;
        }

        if (be32_to_cpu(partSig) == NAND_VAR_BLOCK_SIG_SOLO)
        {
            DBG_PRINTF(NVNAND_DBG_LVL, "Found Solo signature on page - adjust\n");
            bytesScan = getDyPageSize();
            bPageAlign = true;
        }
        else if (be32_to_cpu(partSig) != NAND_VAR_BLOCK_SIG)
        {
            /* Second block not used either: Set active block to 0 and erase both */ 
            DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): No active blocks found, erase and use block 0\n", __LINE__);
            ActiveBlock = 0;
            ErasePartition(handle, UINT64_C(0), getDyPartitionLength());
            ErasePartition(handle, getDyPartitionLength(), getDyPartitionLength());

            /* Write the partition signature - this signature means - built from scratch - on page by itself.*/
            partSig = cpu_to_be32(NAND_VAR_BLOCK_SIG_SOLO);
            activeBlockOffset = UINT64_C(0);
            partMgrErr = WritePartition(handle, (int8_t *)&partSig, activeBlockOffset, sizeof(uint32_t), &len);
            if ((partMgrErr != PARTMGR_OK) || (len != sizeof(uint32_t)))
            {
                DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): Device write FAIL\n", __LINE__);
                goto EXIT;
            }

            /* No need to word align, we are already on word boundary */
            bPageAlign = true;
            setDyWritePtrNAND(0 + sizeof(uint32_t), bPageAlign);
           
            /* Bail out now, no variable blocks to find */
            retStatus = OK;
            goto EXIT;
        }
        else
        {
            /* Found block signature in second block, so set as active data block */
            ActiveBlock = 1;
        }
    }
    else
    {
        /* Found block signature in first block, so set as active data block */
        ActiveBlock = 0;
    }

    /* If we got here then there was an active block that needs scanning, so set
     * an active offset. Note that offsets are relative to the container partition.
     */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();
    
    DBG_PRINTF(NVNAND_DBG_LVL, "NV_DEV_NAND(%d): active block=%d, startOffset=0x%x\n", 
                 __LINE__, ActiveBlock, (uint32_t)activeBlockOffset);
    /* Now scan through the active block looking for variable blocks */
//mikee - test last condition... what does it mean for nand??
    while ((bytesScan < getDyPartitionLength()) &&
           (getDyPartitionLength() - bytesScan + 1 > sizeof(DY_HDR_t))) // are we close to the end?
    {
        /* Read in the variable block data header at current partition read location */
RETRY:
        partMgrErr = ReadPartition(handle, (int8_t *)&dyHeader, activeBlockOffset + bytesScan, sizeof(DY_HDR_t), &len);
        if ((partMgrErr != PARTMGR_OK) || (len != sizeof(DY_HDR_t)))
        {
            DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): device read failure\n", __LINE__);
            goto EXIT;
        }
        /* check for end markers.  Any 0xFF in BlockLbl is invalid. 0xFFFFFFFF as offset or length is invalid... */
        if ( ( UNUSED_DATA8 == dyHeader.BlockLbl[0]) || ( UNUSED_DATA32 == be32_to_cpu(dyHeader.StartOffset) ) || ( UNUSED_DATA32 == be32_to_cpu(dyHeader.MaxLength) ) )
        {
            /* found end of data in this page, let routine calculate our NAND page adjusted pointer */
            if (!bPageAlign)
            {
                DBG_PRINTF(NVNAND_DBG_LVL,"NV_DEV_NAND(%d): try next page once\n", __LINE__);
                bPageAlign = true;
                setDyWritePtrNAND(bytesScan, bPageAlign);
                bytesScan = getDyWritePtrNAND();
                goto RETRY;
            }
            /* An erased header marks the normal end of the table */
            DBG_PRINTF(LOG_INFO,"NV_DEV_NAND(%d): found erased data, stopping table scan\n", __LINE__);
            break;
        }
        else if ( ( 0 == be32_to_cpu(dyHeader.StartOffset) ) || ( 0 == be32_to_cpu(dyHeader.MaxLength) ) ||
                  ( be32_to_cpu(dyHeader.StartOffset) > getDyPartitionLength()) )
        {
            /* Current variable block looks corrupt:
             *   - startoffset should always point around the header (at least 12 bytes)
             *   - we do not allow 0 length blocks
             *   - the offset extends beyond the container
             * Attempt to switch to backup block (which will chop off any blocks after the
             * current scan point).
             */
            DBG_PRINTF(LOG_INFO,"NV_DEV_NAND(%d): Block may be corrupt, switch to backup\n", __LINE__);
            if (OK != SwitchToBackupBlock(memDev))
            {
                /* We are really in the ditch, perhaps just a full erase here? */
                ASSERT(false);
                goto EXIT;
            }
            
            /* The switch block routine will have set everything, so bail out now */
            retStatus = OK;
            goto EXIT;
        }

        /* Update the bytes scanned in current block */
        bytesScan = be32_to_cpu(dyHeader.StartOffset) + be32_to_cpu(dyHeader.MaxLength);

        /* let routine calculate our NAND page adjusted pointer */
        setDyWritePtrNAND(bytesScan, bPageAlign);
        bytesScan = getDyWritePtrNAND();

        /* In NAND we can end up with several versions of a var block in the active 
         * block (unlike EEPROM which only has only one copy) so we can skip any 
         * instances that are corrupt.  To verify the block checksum we need to
         * allocate a buffer and read in the block data.
         */
        blockPtr = (VARBLOCK_HEADER_t *)MEM_MALLOC( be32_to_cpu(dyHeader.MaxLength) );
        if (blockPtr == NULL)
        {
            goto EXIT;
        }

        partMgrErr = ReadPartition(handle,
                (int8_t *)blockPtr,
                activeBlockOffset + be32_to_cpu(dyHeader.StartOffset),
                be32_to_cpu(dyHeader.MaxLength),
                &len);

        if ( ( partMgrErr != PARTMGR_OK ) || ( len != be32_to_cpu(dyHeader.MaxLength) ) )
        {
            DBG_PRINTF(LOG_ERR, "NV_DEV_NAND(%d): device read failure\n", __LINE__);
            goto EXIT;
        }
        
    /* Process the block if the CRC is OK, otherwise skip it; check block length before running CRC as a
       sanity check for corrupt memory.  Block length should be <= partition length. */
        if ( blockPtr->Len <= getDyPartitionLength() && VarBlockCheckCRC(&dyHeader.BlockLbl[0], blockPtr, UNUSED_DATA32) )
        {
            /* Check to see if a partition with BlockLbl already exists */
            newPartEntry = FindNodeByLabelAndDev(dyHeader.BlockLbl, memDev, VarBlockListNAND);
            if (newPartEntry != NULL)  
            {
                /* Entry exists (found a newer version): remove it from list for an update */
                ATRemoveEntryList(&newPartEntry->listNode);
                DBG_PRINTF(NVNAND_DBG_LVL, "NV_DEV_NAND(%d): updating NAND var block '%4s'\n", __LINE__, newPartEntry->Label);
            }
            else
            {
                /* Entry does not exist: Allocate new entry and fill it out */
                newPartEntry = AllocNewNode();
                if (newPartEntry == NULL)
                {
                    ASSERT(false);  
                    goto EXIT;
                }
                memcpy(&newPartEntry->Label[0], &dyHeader.BlockLbl[0], 4);
                DBG_PRINTF(NVNAND_DBG_LVL, "NV_DEV_NAND(%d): adding NAND var block '%4s'\n", __LINE__, newPartEntry->Label);
            }

            /* Fill out the list entry (only assume the block label is valid) */
            newPartEntry->StartOffset = be32_to_cpu(dyHeader.StartOffset);
            newPartEntry->Size        = be32_to_cpu(dyHeader.MaxLength);
            newPartEntry->MemDev      = memDev;
            newPartEntry->Committed   = true;
       
            /* Insert into partition list */        
            ATInsertTailList(VarBlockListNAND, &newPartEntry->listNode);
        }
        else
        {
            DBG_PRINTF(NVNAND_DBG_LVL,"NV_DEV_NAND(%d): bad CRC, ignore var block '%4s'\n", __LINE__, dyHeader.BlockLbl);
        }
        
        MEM_FREE_AND_NULL(blockPtr);
    }

    /* Scan complete: The next variable block will be written at bytesScan (relative to active block) */
    setDyWritePtrNAND(bytesScan, true);
    
    /* We made it, set OK return status */
    retStatus = OK;
    
EXIT:
    if (handle != NULL)
    {
        if (ClosePartition(handle) != PARTMGR_OK)
        {
            ASSERT(false);
        }
    }
    
    if (blockPtr != NULL)
    {
        MEM_FREE_AND_NULL(blockPtr);
    }
    
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramNAND );
    XASSERT( px_status==0, px_status );
        
    return retStatus;
}


/**
 *  \brief Switch content from active to backup block
 *
 *  This is called when there is no space to write new NVRAM data in the active
 *  block of the container.  We need to copy the lastest version of NVRAM data
 *  to a local buffer and creates a new partition list. The data buffers is then
 *  written to the backup block.  The backup block then becomes the active block.
 *
 *  \warning
 *  The caller needs to hold the state variable protection mutex!
 *
 *  \param[in] memDev  The device hosting the container partition
 *
 *  \return error_type_t
 **/
static error_type_t SwitchToBackupBlock(MEMORY_DEVICE memDev)
{
    error_type_t        retStatus = FAIL;
    BLOCK_NODE_t       *myNode;
    ATLISTENTRY        *searchNode;
    uint32_t            blockSig;
    uint32_t            backupBlock;
    PARTMGR_ERROR_t     partErr;
    uint32_t            len;
    uint32_t            committedSize;
    uint64_t            nodeOffset;
    uint64_t            activeBlockOffset;
    partition_handle_t *pHandle      = NULL;
    int8_t             *buff         = NULL;
    ATLISTENTRY        *newBlockList = NULL;
    uint32_t            bytesRead    = 0;

    /* NOTE:
     * A failure in this routine can have a huge impact, so try and alloc
     * all needed memory up front.  Also try and keep an exit open that 
     * leaves the current system state intact (which may still be unusable).
     */
     
    /* Scan the list for committed blocks and get the total size: should be
     * much less than the actual active block size.  Allocate a buffer for
     * our ram copy.
     */
    committedSize = GetCommittedSizeNAND(VarBlockListNAND);
    buff = (int8_t *)MEM_MALLOC(committedSize);
    if (buff == NULL)
    {
        goto EXIT;
    }
    memset(buff, 0xff, committedSize);

    /* Clone the current block list: need to keep the current list intact in case
     * we fail and bail out.
     */
    newBlockList = CloneNodeList(VarBlockListNAND); 
    if (newBlockList == NULL)
    {
        goto EXIT;
    }
       
    /* All working RAM allocated, now open our 'container' partition */
    if (PARTMGR_OK != OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &pHandle))
    {
        goto EXIT;
    }

    /* Calculate offset to the start of the active block */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();

    /* Read first four bytes of active block, it should be the block signature */
    partErr = ReadPartition(pHandle, &buff[bytesRead], activeBlockOffset, sizeof(uint32_t), &len);
    if ((partErr != PARTMGR_OK) || (len != sizeof(uint32_t)))
    {
        goto EXIT;
    }

    /* Verify active block signature */
    blockSig = be32_to_cpu( *((uint32_t *)buff) );
    if ((blockSig != NAND_VAR_BLOCK_SIG) && (blockSig != NAND_VAR_BLOCK_SIG_SOLO))
    {
        /* We have big trouble now: the active block is bogus.  Place the signature
         * in our working buffer and dump the current lists (they can't be trusted).
         */
        DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): active block corrupt\n", __LINE__);
        *((uint32_t *)buff) = cpu_to_be32(NAND_VAR_BLOCK_SIG);
        //mikee - test - what happens if not data is written with signature - it should be ALONE.
        DumpNodeList(VarBlockListNAND);
        DumpNodeList(newBlockList);
    }    

    /* Move buffer 'pointer' past the block signature */
    bytesRead += sizeof(uint32_t);

    /* Now work through the cloned list, read the valid data into our ram buffer */
    searchNode = newBlockList->nextEntry;
    while (searchNode != newBlockList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if (myNode != NULL)
        {
            /* Only have to move data for committed blocks */
            if (myNode->Committed == true)
            {
                /* Verify our buffer state in development: catch an overflow */
                ASSERT((bytesRead + myNode->Size + sizeof(DY_HDR_t)) <= committedSize);

                /* Get offset to start of current var block */
                nodeOffset = (myNode->StartOffset - sizeof(DY_HDR_t));

                /* Read var block data into ram buffer */
                partErr = ReadPartition(pHandle, &buff[bytesRead], (activeBlockOffset + nodeOffset), (myNode->Size + sizeof(DY_HDR_t)), &len);
                if ((partErr != PARTMGR_OK) || (len != (myNode->Size + sizeof(DY_HDR_t))))
                {
                    DBG_PRINTF(LOG_ERR,"NV_DEV_NAND(%d): SwitchToBackupBlock unable to read data from partition\n", __LINE__);
                    goto EXIT;
                }

                /* Offsets in the new container will be different: must adjust the list
                 * and buffer offsets to new location
                 */
                myNode->StartOffset = bytesRead + sizeof(DY_HDR_t);
                *((uint32_t *)(buff + bytesRead + NAND_OFFSET_OFFSET)) = cpu_to_be32( (uint32_t)(myNode->StartOffset) );

                /* Move bytesRead to next write position, make sure it's word aligned */
                bytesRead += (sizeof(DY_HDR_t) + myNode->Size);
                if (bytesRead & 0x00000003)
                {
                    bytesRead += 4;
                    bytesRead &= 0xFFFFFFFC;
                }                
            }
        }
        
        searchNode = searchNode->nextEntry;
    }
    
    /* Identify backup block */
    backupBlock = (ActiveBlock == 0) ? 1 : 0; 

    /* We need to copy our RAM buffer to the backup block, but this is
     * bound to be a rather large copy operation.  A power fail in the 
     * middle could leave us with two signed blocks, maybe a future task, 
     * but NAND is faster than SPI, so all in one might be ok.
     */

    activeBlockOffset = backupBlock * getDyPartitionLength();
    /* 
     * If there is no data to be copied, skip down to the signature copy
     */
    if( sizeof(uint32_t) == bytesRead )
    {
        *((uint32_t *)buff) = cpu_to_be32(NAND_VAR_BLOCK_SIG_SOLO);
    }
    else
    {
        *((uint32_t *)buff) = cpu_to_be32(NAND_VAR_BLOCK_SIG);
    }
    partErr = WritePartition(pHandle, buff, activeBlockOffset, bytesRead, &len);
    if ((partErr != PARTMGR_OK) || (len != bytesRead))
    {
        /* Make sure backup block is still clean and exit */
        DBG_PRINTF(LOG_ERR, "NV_DEV_NAND(%d): unable to write data to backup partition\n", __LINE__);
        activeBlockOffset = backupBlock * getDyPartitionLength();
        ErasePartition(pHandle, activeBlockOffset, getDyPartitionLength());
        ASSERT(false);
        goto EXIT;
    }

    /* Erase active block (data already copied to backup block) */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();
    ErasePartition(pHandle, activeBlockOffset, getDyPartitionLength());

    /* Make the cloned list the master */
    DumpNodeList(VarBlockListNAND);
    MEM_FREE_AND_NULL(VarBlockListNAND);
    VarBlockListNAND = newBlockList;
    newBlockList = NULL;

    /* Switch the active block */
    ActiveBlock = backupBlock;

    /* Update dynamic write pointer (relative to active block) */
    setDyWritePtrNAND(bytesRead, true);

    DBG_PRINTF(LOG_INFO, "NV_DEV_NAND(%d): Switched active block to %d on %s\n", 
                __LINE__, ActiveBlock, getDevIdStr(memDev));
    retStatus = OK;

EXIT:
    if (pHandle != NULL)
    {
        ClosePartition(pHandle);
    }
    
    if (buff != NULL)
    {
        MEM_FREE_AND_NULL(buff);
    }
 
    if (newBlockList != NULL)
    {
         DumpNodeList(newBlockList);
         MEM_FREE_AND_NULL(newBlockList);
    }
   
    return retStatus;
}


/**
 *
 *  \brief Calculate total space used by committed blocks
 *
 *  Note that the caller must hold the state variable protection mutex.
 *
 **/
static uint32_t GetCommittedSizeNAND(ATLISTENTRY *theList)
{
    BLOCK_NODE_t *myNode;
    ATLISTENTRY  *searchNode;
    uint32_t      theSize = sizeof(uint32_t);
    
    if (theList == NULL)
    {
        /* List is missing, only have block signature */
        return theSize;
    }

    /* We have at least one entry in the table */
    searchNode = theList->nextEntry;
    while (searchNode != theList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if ((myNode != NULL) && (myNode->Committed == true))
        {
            /* Each block has a committed size, header, and worst case alignment */
            theSize += myNode->Size;
            theSize += sizeof(DY_HDR_t);
            theSize += sizeof(uint32_t);
        }
        searchNode = searchNode->nextEntry;        
    }
    
    return theSize;
}


/**
 *
 *  \brief Returns pointer to next dynamic write position
 *
 *  \return uint32_t
 *  \retval Pointer to dynamic write pointer
 *
 **/
static uint32_t getDyWritePtrNAND(void)
{
    return DyWriteOffsetNAND;
}


/**
 *
 *  \brief Sets pointer to next dynamic write position
 *
 *  \param [in] WriteOffset
 *
 *  \return error_type_t
 **/
static error_type_t setDyWritePtrNAND(uint32_t WriteOffset, bool forcePageAlign)
{
    DBG_PRINTF(NVNAND_DBG_LVL, "%s was %#x\n", __FUNCTION__, DyWriteOffsetNAND);
    /* Word align write pointer */
    if (WriteOffset & 0x00000003)
    {
        WriteOffset += 4;
        WriteOffset &= 0xFFFFFFFC;
    }
    /* force into next page, unless we are at offset 0 of next available page */
    if (forcePageAlign)
    {
        uint32_t ps = getDyPageSize();
        ASSERT(ps);
        if (WriteOffset & (ps-1))
        {
            WriteOffset += (ps-1);   
            WriteOffset &= ~(ps-1);
        }
    }
            
    DyWriteOffsetNAND = WriteOffset;
    DBG_PRINTF(NVNAND_DBG_LVL, "%s now %#x\n", __FUNCTION__, DyWriteOffsetNAND);
            
    return OK;
}

static void setDyPartitionLength(uint32_t Length)
{
    MaxDyPartionLen = Length;
}

static uint32_t getDyPartitionLength(void)
{
    return MaxDyPartionLen;
}

static void setDyPageSize(uint32_t Length)
{
    DyNandPageSize = Length;
}

static uint32_t getDyPageSize(void)
{
    return DyNandPageSize;
}


#ifdef HAVE_CMD 

/* Prints list of variable blocks */

static void ListBlocks(ATLISTENTRY *blkList)
{
    BLOCK_NODE_t *myNode;
    ATLISTENTRY  *searchNode;
    
    if (blkList == NULL)
    {
        return;
    }
  
    DPRINTF(DBGOPT, ("Active block=%d Write Ptr=0x%x Max Partition Length=0x%x\n\n",
                  ActiveBlock, getDyWritePtrNAND(), getDyPartitionLength()));
  
    searchNode = blkList->nextEntry;
    if ( searchNode == blkList )
    {
        DBG_PRINTF(LOG_INFO, "Partition Table is Empty\n");
        return;
    }

    while (searchNode != blkList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if (myNode != NULL)
        {
            DPRINTF(DBGOPT,("%s Block :  ", getDevIdStr(myNode->MemDev)));
            DPRINTF(DBGOPT,("'%c%c%c%c'", myNode->Label[0],myNode->Label[1],
                     myNode->Label[2],myNode->Label[3]));
            DPRINTF(DBGOPT,(" offset=0x%x, block size=0x%x\n", 
                     (uint32_t)myNode->StartOffset, (uint32_t)myNode->Size));
        }
        searchNode = searchNode->nextEntry;
    }
}
  
static int NVNAND_callback( int argc, char *argv[] )
{
    /* 
     *  Look at argv[1] for a few simple commands that we can use to 
     *  test the api. 
     */
    if ( strncmp( argv[1], "list", 4 ) == 0 )
    {
        ListBlocks(VarBlockListNAND);
    }
    else if ( strncmp( argv[1], "clr", 3 ) == 0 )
    {
        DumpNodeList(VarBlockListNAND);
    }
    else if ( strncmp( argv[1], "swap", 4 ) == 0 )
    {
        SwitchToBackupBlock(MEM_NAND0);        
    }
    else if ( strncmp( argv[1], "erase", 5 ) == 0 )
    {
        partition_handle_t *pHandle      = NULL;
        /* All working RAM allocated, now open our 'container' partition */
        if (PARTMGR_OK == OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, MEM_NAND0, &pHandle))
        {
            ErasePartition(pHandle, UINT64_C(0), getDyPartitionLength());
            ErasePartition(pHandle, getDyPartitionLength(), getDyPartitionLength());
        }
    }
    else if ( strncmp( argv[1], "dup", 3 ) == 0 )
    {
        if(argc == 4)
        {
            nvram_handle_t   nHandle;
            uint32_t block;
            nHandle.BlockLabel[0]=argv[2][0];
            nHandle.BlockLabel[1]=argv[2][1];
            nHandle.BlockLabel[2]=argv[2][2];
            nHandle.BlockLabel[3]=argv[2][3];
            str_mkint( argv[3], &block );

            nHandle.DeviceID=MEM_NAND0;
            nHandle.MaxLength=1024;
            int8_t data[1024];
            nHandle.MaxLength = VarBlockReadNAND(&nHandle, data, 0, block);
            VarBlockWriteNAND(&nHandle, data);
        }
    }
    else
    {
        DPRINTF(DBGOPT,("\n*** NV DEV NAND Commands ***\n"));
        DPRINTF(DBGOPT,(" list:  Lists current variable blocks\n"));
        DPRINTF(DBGOPT,(" clr:   Clears the list of current variable blocks\n"));
        DPRINTF(DBGOPT,(" swap:  Switches to the backup block\n"));
        DPRINTF(DBGOPT,(" erase: Erase Active & Backup block\n"));
        DPRINTF(DBGOPT,(" dup 'xxxx' Find an entry 'xxxx' & duplicate\n"));
        DPRINTF(DBGOPT,("     use 'list' prior to get correct block size\n"));
        DPRINTF(DBGOPT,("     dup PLAT 0x28\n"));
        DPRINTF(DBGOPT,("\n"));
    }

    return 0;
}

#endif /* DEBUG */
