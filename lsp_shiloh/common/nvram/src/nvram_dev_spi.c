/**
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_dev_spi.c
 *
 *  \brief NVRAM SPI device shim layer
 *
 *  This file manages blocks of NVRAM variables on SPI devices. The variable
 *  block storage algorithm is designed specifically to do wear leveling on
 *  SPI devices:
 *
 *  Container Partition -->   +------------------------+
 *  Start                     | - Block Signature      |    (Active Block = 0)
 *                            | - 'PRNT' Vars          |
 *                            | - 'SCAN' Vars          |
 *                            | - 'PLAT' Vars          |
 *                            | - 'SCAN' Vars (updated)|
 *       DyWriteOffsetSPI --> |    (Erased Space)      |
 *                            |                        |
 *                            |                        |
 *                       +--> +------------------------+
 *                       |    |    (Erased Space)      |    (Backup Block = 1)
 *                       |    |                        |
 *                       |    |                        |
 *    MaxDyPartionLen ---|    |                        |
 *                       |    |                        |
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
#include "nvram_dev_spi.h"
#include "nvram_logger.h"
#include "ioutils.h"

#ifdef DEBUG
    #include "debug.h"
#endif

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

/*
 * Each 'dynamic' block record in SPI has a header three 32-bit words long:
 *    +-----------+------------+----------+----------------------------
 *    |    ID     |   LENGTH   |  OFFSET  |  Length bytes of Data ...
 *    |    (0)    |    (1)     |   (2)    |
 *    +-----------+------------+----------+----------------------------
 */
#define SPI_ID_OFFSET          0           /**< Offset in bytes to ID field       */
#define SPI_LEN_OFFSET         4           /**< Offset in bytes to Length field   */
#define SPI_OFFSET_OFFSET      8           /**< Offset in bytes to offset field   */
#define SPI_HDR_LEN            12          /**< Header length in bytes            */

#define UNUSED_DATA32          0xffffffff  /**< Unused/erased SPI data            */

#define MIN_SPI_PARTSIZE       32768       /**< Smallest partition size, in bytes */

#define SPI_VAR_BLOCK_SIG      0xFACE      /**< Container partition signature     */

/*
 * Local variable types
 */

typedef struct                     /* Note: sizeof(DY_HDR_t) = SPI_HEADER_LEN     */
{
    char      BlockLbl[4];         /**< Variable block label (4 chars, 32 bits)   */
    uint32_t  MaxLength;           /**< Maximum length of variable block          */
    uint32_t  StartOffset;         /**< Offset from the start of active partition */
} DY_HDR_t;


/*
 * Static variable declarations
 */
static pthread_mutex_t mutex_nvramSPI;
static uint32_t      DyWriteOffsetSPI = 0;      /* Points to next write offset in SPI memory */
static ATLISTENTRY  *VarBlockListSPI = NULL;    /* List of variable blocks on SPI            */
static uint32_t      MaxDyPartionLen = 0;       /* Maximum length of dynamic partition       */
static uint32_t      ActiveBlock     = 0;       /* Active data block in container partition  */
static bool          SpiInitComplete = false;

/*
 * Static function declarations
 */
static error_type_t setDyWritePtrSPI(uint32_t);
static uint32_t     getDyWritePtrSPI(void);
static void         setDyPartitionLength(uint32_t);
static uint32_t     getDyPartitionLength(void);
static error_type_t SwitchToBackupBlock(MEMORY_DEVICE memDev);
static uint32_t     GetCommittedSizeSPI(ATLISTENTRY *theList);


#ifdef HAVE_CMD
static void ListBlocks(ATLISTENTRY *blkList);
static int nvspi_cmd_list_cb(int argc, char *argv[]);
static int nvspi_cmd_clr_cb(int argc, char *argv[]);
static int nvspi_cmd_swap_cb(int argc, char *argv[]);
static const char *nvspi_cmd_list_desc;
static const char *nvspi_cmd_list_usage;
static const char *nvspi_cmd_list_notes;
static const char *nvspi_cmd_clr_desc;
static const char *nvspi_cmd_clr_usage;
static const char *nvspi_cmd_clr_notes;
static const char *nvspi_cmd_swap_desc;
static const char *nvspi_cmd_swap_usage;
static const char *nvspi_cmd_swap_notes;
#endif


/*
 * FUNCTION NAME: InitNvramSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
void InitNvramSPI(void)
{
    int px_status;
#ifdef HAVE_CMD
    int cmd_res;
#endif

    /* Make sure our constants match the actual data types */
    ASSERT(SPI_HDR_LEN == sizeof(DY_HDR_t));

    /* Only run through init code once */
    if (SpiInitComplete == false)
    {
        SpiInitComplete = true;

        VarBlockListSPI = (ATLISTENTRY *)MEM_MALLOC(sizeof(ATLISTENTRY));
        ASSERT(VarBlockListSPI != NULL);

        /* Initialize variable block list */
        ATInitList(VarBlockListSPI);

        /* Create mutex for protecting the SPI write pointer */
        px_status = posix_mutex_init( &mutex_nvramSPI );
        XASSERT( px_status==0, px_status );

#ifdef HAVE_CMD
        cmd_res = cmd_register_cmd( "nvspi",
                                    NULL,
                                    "nvram spi related commands",
                                    NULL,
                                    NULL,
                                    NULL );
        ASSERT( CMD_OK == cmd_res );
        cmd_res = cmd_register_subcmd( "nvspi",
                                       "list",
                                       nvspi_cmd_list_desc,
                                       nvspi_cmd_list_usage,
                                       nvspi_cmd_list_notes,
                                       nvspi_cmd_list_cb );
        ASSERT( CMD_OK == cmd_res );
        cmd_res = cmd_register_subcmd( "nvspi",
                                       "clr",
                                       nvspi_cmd_clr_desc,
                                       nvspi_cmd_clr_usage,
                                       nvspi_cmd_clr_notes,
                                       nvspi_cmd_clr_cb );
        ASSERT( CMD_OK == cmd_res );
        cmd_res = cmd_register_subcmd( "nvspi",
                                       "swap",
                                       nvspi_cmd_swap_desc,
                                       nvspi_cmd_swap_usage,
                                       nvspi_cmd_swap_notes,
                                       nvspi_cmd_swap_cb );
        ASSERT( CMD_OK == cmd_res );
#endif
    }
}


/*
 * FUNCTION NAME: GetPartitionNeedsSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
error_type_t GetPartitionNeedsSPI(MEMORY_DEVICE      memDev, 
                                  uint32_t          *partitionSize)
{
    uint32_t   spiBlockCount;
    uint32_t   spiBlockSize;
    uint32_t   spiPageSize;
    uint32_t   neededBlocks;

    /* Note: no state variables used in this API routine, so mutex protection
     *       is not required.
     */
             
    /* Get the SPI part block size */
    if (PARTMGR_OK != GetDeviceInfo(memDev, &spiBlockCount, &spiBlockSize, &spiPageSize))
    {
        ASSERT(false);
        return FAIL;
    }

    /* Calculate the needed number of blocks to meet the minimum container
     * partition size 
     */    
    neededBlocks = MIN_SPI_PARTSIZE / spiBlockSize;
    if (MIN_SPI_PARTSIZE % spiBlockSize)
    {
        /* Round up if the requested size falls into next block (or fell short) */
        neededBlocks += 1;
    }

    /* Double the space for our wear leveling algorithm (active/backup areas) */
    neededBlocks *= 2;

    /* Set return values */
    *partitionSize = neededBlocks * spiBlockSize;

    return OK;
}


/*
 * FUNCTION NAME: GetVarBlockSizeSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
uint32_t GetVarBlockSizeSPI(MEMORY_DEVICE memDev, char *label)
{
    int px_status;
    BLOCK_NODE_t *myNode;
    uint32_t      retSize = 0;
 
    /* Acquire mutex */
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }

    /* Look for the var block, set size if found */
    myNode = FindNodeByLabelAndDev(label, memDev, VarBlockListSPI);
    if (myNode != NULL)
    {
        retSize = myNode->Size;
    }
 
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
    XASSERT( px_status==0, px_status );

    return retSize;
}


/*
 * FUNCTION NAME: VarBlockReadSPI
 *
 *   See nvram_dev_spi.h for function documentation.
 */
uint32_t VarBlockReadSPI(nvram_handle_t   *nHandle, 
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
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }
    
    /* Find the entry in the variable block list */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, VarBlockListSPI);

    /* Can't read data if it doesn't exist or hasn't been committed (written) */
    if ((partEntry == NULL) || (partEntry->Committed == false))
    {
        goto EXIT;
    }

    /* Make sure we aren't reading off end of our var block */
    if ((offset + size) > partEntry->Size)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): attempt to read past end of var block\n", __LINE__);
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
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): device read failure\n", __LINE__);
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
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
    XASSERT( px_status==0, px_status );
       
    return len;
}


/*
 * FUNCTION NAME: VarBlockWriteSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
uint32_t VarBlockWriteSPI(nvram_handle_t *nHandle, int8_t *Src)
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
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return 0;
    }

    /* Get the SPI write pointer (next write address). It will be word aligned. */
    writeOffset = getDyWritePtrSPI();
    ASSERT(writeOffset != 0);
    
    /* Get the actual write length (block size plus our SPI header) */
    writeLen = nHandle->MaxLength + sizeof(DY_HDR_t);
    
    /* Check if write would hit end of the container partition space */
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
        writeOffset = getDyWritePtrSPI();
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
         DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): unable to allocate memory for data buffer\n", __LINE__);
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
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): device write failure\n", __LINE__);
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
    setDyWritePtrSPI(writeOffset + writeLen);
    
    /* Now our block is committed to the NV store, find the entry in the var block
     * list and modify to reference new offset
     */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, VarBlockListSPI);
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
    ATInsertTailList(VarBlockListSPI, &partEntry->listNode);
        
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
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
    XASSERT( px_status==0, px_status );
       
    return retLen;
}


/*
 * FUNCTION NAME: RegisterVarBlockSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
error_type_t RegisterVarBlockSPI(char         *label, 
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
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d):invalid label or size\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Test to see if the var block already exists */
    oldEntry = FindNodeByLabelAndDev(label, memDev, VarBlockListSPI);
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
    ATInsertTailList(VarBlockListSPI, &newEntry->listNode);
    
    retCode = OK;
    
EXIT:
    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
    XASSERT( px_status==0, px_status );
       
    return retCode;
}


/*
 * FUNCTION NAME: ReleaseVarBlockSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
error_type_t ReleaseVarBlockSPI(char         *label, 
                                MEMORY_DEVICE memDev)
{
    int px_status;
    BLOCK_NODE_t *partEntry;
    error_type_t  retStatus = FAIL;

    /* Acquire mutex */
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Attempt to locate the block: remove it from the list if found */
    partEntry = FindNodeByLabelAndDev(label, memDev, VarBlockListSPI);
    if (partEntry != NULL)
    {
        ATRemoveEntryList(&partEntry->listNode);
        MEM_FREE_AND_NULL(partEntry);

        /* We can't simply remove the block from SPI memory (the entire SPI sector must
         * be erased).  Force a switch to the backup block which will not copy the 
         * removed block.
         */
        retStatus = SwitchToBackupBlock(memDev);
    }

    /* Put mutex */
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
    XASSERT( px_status==0, px_status );

    return retStatus;
}


/*
 * FUNCTION NAME: DiscoverVarBlocksSPI
 *
 * See nvram_dev_spi.h for function documentation.
 */
error_type_t DiscoverVarBlocksSPI(MEMORY_DEVICE memDev)
{
    error_type_t        retStatus = FAIL;
    int px_status;
    uint32_t            partSig;
    DY_HDR_t            dyHeader;
    BLOCK_NODE_t       *newPartEntry;
    uint64_t            activeBlockOffset;
    uint32_t            spiBlockCount;
    uint32_t            spiBlockSize;
    uint32_t            spiPageSize;
    PARTMGR_ERROR_t     partMgrErr;
    partition_info_t    Info;
    uint32_t            len;
    uint32_t            containSize;
    partition_handle_t *handle    = NULL;
    VARBLOCK_HEADER_t  *blockPtr  = NULL;
    uint32_t            bytesScan = sizeof(uint32_t);

    /* Acquire mutex.  Note that once we have the mutex this routine must
     * return through EXIT to ensure the mutex is put back.
     */
    px_status = pthread_mutex_lock( &mutex_nvramSPI );
    if (px_status != 0)
    {
        ASSERT(false);
        return FAIL;
    }
    
    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): Discover NV blocks on %s\n", __LINE__, getDevIdStr(memDev));
    
    /* Open our 'container' partition */
    if (PARTMGR_OK != OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &handle))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): NV container partition not present on %s\n",
                      __LINE__, getDevIdStr(memDev));
        goto EXIT;
    }

    /* Get the partition information */
    if (PARTMGR_OK != GetPartitionInfoByHandle(handle, &Info))
    {
        ASSERT(false);
        goto EXIT;
    }

    /* Get the SPI part block size */
    if (PARTMGR_OK != GetDeviceInfo(memDev, &spiBlockCount, &spiBlockSize, &spiPageSize))
    {
        ASSERT(false);
        goto EXIT;
    }
    
    /* While we should have validated the container before now, going to double
     * check we are at least aligned properly.  Note we divide the container 
     * size by 2 to get active/backup sizes (the actual usable size).
     */
    containSize = Info.Size / 2;
    if (containSize % spiBlockSize)
    {
        /* We should be block boundary aligned ... */
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): container not properly aligned\n", __LINE__);
        ASSERT(false);
        goto EXIT;
    }

    /* Update container partition length */
    setDyPartitionLength(containSize);

    /* Our variable block list should be empty at this point, but make sure */
    DumpNodeList(VarBlockListSPI);

    /* We need to find the active block. Read in the first 4 bytes of block 0
     * to for look for the block signature 
     */
    activeBlockOffset = UINT64_C(0);
    partMgrErr = ReadPartition(handle, (int8_t *)&partSig, activeBlockOffset, bytesScan, &len);
    if ((partMgrErr != PARTMGR_OK) || (len != bytesScan))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): device read failure\n", __LINE__);
        goto EXIT;
    }

    if (be32_to_cpu(partSig) != SPI_VAR_BLOCK_SIG)
    {
        /* No signature in block 0, read the next container boundary */
        activeBlockOffset = getDyPartitionLength();
        partMgrErr = ReadPartition(handle, (int8_t *)&partSig, activeBlockOffset, bytesScan, &len);
        if ((partMgrErr != PARTMGR_OK) || (len != bytesScan))
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): device read failure\n", __LINE__);
            goto EXIT;
        }

        if (be32_to_cpu(partSig) != SPI_VAR_BLOCK_SIG)
        {
            /* Second block not used either: Set active block to 0 and erase both */ 
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): No active blocks found, erase and use block 0\n", __LINE__);
            ActiveBlock = 0;
            ErasePartition(handle, UINT64_C(0), getDyPartitionLength());
            ErasePartition(handle, getDyPartitionLength(), getDyPartitionLength());

            /* Write the partition signature */
            partSig = cpu_to_be32(SPI_VAR_BLOCK_SIG);
            activeBlockOffset = UINT64_C(0);
            partMgrErr = WritePartition(handle, (int8_t *)&partSig, activeBlockOffset, sizeof(uint32_t), &len);
            if ((partMgrErr != PARTMGR_OK) || (len != sizeof(uint32_t)))
            {
                DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): Device write FAIL\n", __LINE__);
                goto EXIT;
            }

            /* No need to word align, we are already on word boundary */
            setDyWritePtrSPI(0 + sizeof(uint32_t));
           
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
    
    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): active block=%d, startOffset=0x%x\n", 
                 __LINE__, ActiveBlock, (uint32_t)activeBlockOffset);

    /* Now scan through the active block looking for variable blocks */
    while ((bytesScan < getDyPartitionLength()) &&
           (getDyPartitionLength() - bytesScan + 1 > sizeof(DY_HDR_t))) // are we close to the end?
    {
        /* Read in the variable block data header at current partition read location */
        partMgrErr = ReadPartition(handle, (int8_t *)&dyHeader, activeBlockOffset + bytesScan, sizeof(DY_HDR_t), &len);
        if ((partMgrErr != PARTMGR_OK) || (len != sizeof(DY_HDR_t)))
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): device read failure\n", __LINE__);
            goto EXIT;
        }

        if ( ( UNUSED_DATA32 == be32_to_cpu(dyHeader.StartOffset) ) && ( UNUSED_DATA32 == be32_to_cpu(dyHeader.MaxLength) ) )
        {
            /* An erased header marks the normal end of the table */
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): found erased data, stopping table scan\n", __LINE__);
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
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): Block may be corrupt, switch to backup\n", __LINE__);
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

        /* Word align bytesScan to read the next header at word boundary */
        if (bytesScan & 0x00000003)
        {
            bytesScan += 4;
            bytesScan &= 0xFFFFFFFC;
        }

        /* In SPI we can end up with several versions of a var block in the active 
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
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): device read failure\n", __LINE__);
            goto EXIT;
        }
        
	/* Process the block if the CRC is OK, otherwise skip it; check block length before running CRC as a
	   sanity check for corrupt memory.  Block length should be <= partition length. */
        if ( blockPtr->Len <= getDyPartitionLength() && VarBlockCheckCRC(&dyHeader.BlockLbl[0], blockPtr, UNUSED_DATA32) )
        {
            /* Check to see if a partition with BlockLbl already exists */
            newPartEntry = FindNodeByLabelAndDev(dyHeader.BlockLbl, memDev, VarBlockListSPI);
            if (newPartEntry != NULL)  
            {
                /* Entry exists (found a newer version): remove it from list for an update */
                ATRemoveEntryList(&newPartEntry->listNode);
                DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): updating SPI var block '%4s'\n", __LINE__, newPartEntry->Label);
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
                memcpy(newPartEntry->Label, dyHeader.BlockLbl, 4);
                DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): adding SPI var block '%4s'\n", __LINE__, newPartEntry->Label);
            }

            /* Fill out the list entry (only assume the block label is valid) */
            newPartEntry->StartOffset = be32_to_cpu(dyHeader.StartOffset);
            newPartEntry->Size        = be32_to_cpu(dyHeader.MaxLength);
            newPartEntry->MemDev      = memDev;
            newPartEntry->Committed   = true;
       
            /* Insert into partition list */        
            ATInsertTailList(VarBlockListSPI, &newPartEntry->listNode);
        }
        else
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): bad CRC, ignore var block '%4s'\n", __LINE__, dyHeader.BlockLbl);
        }
        
        MEM_FREE_AND_NULL(blockPtr);
    }

    /* Scan complete: The next variable block will be written at bytesScan (relative to active block) */
    setDyWritePtrSPI(bytesScan);
    
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
    px_status = pthread_mutex_unlock( &mutex_nvramSPI );
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
    int8_t             *wrPtr;
    uint32_t            wrLen;
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
    committedSize = GetCommittedSizeSPI(VarBlockListSPI);
    buff = (int8_t *)MEM_MALLOC(committedSize);
    if (buff == NULL)
    {
        goto EXIT;
    }
    memset(buff, 0xff, committedSize);

    /* Clone the current block list: need to keep the current list intact in case
     * we fail and bail out.
     */
    newBlockList = CloneNodeList(VarBlockListSPI); 
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
    if (blockSig != SPI_VAR_BLOCK_SIG)
    {
        /* We have big trouble now: the active block is bogus.  Place the signature
         * in our working buffer and dump the current lists (they can't be trusted).
         */
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): active block corrupt\n", __LINE__);
        *((uint32_t *)buff) = cpu_to_be32(SPI_VAR_BLOCK_SIG);
        DumpNodeList(VarBlockListSPI);
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
                    DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI,"NV_DEV_SPI(%d): SwitchToBackupBlock unable to read data from partition\n", __LINE__);
                    goto EXIT;
                }

                /* Offsets in the new container will be different: must adjust the list
                 * and buffer offsets to new location
                 */
                myNode->StartOffset = bytesRead + sizeof(DY_HDR_t);
                *((uint32_t *)(buff + bytesRead + SPI_OFFSET_OFFSET)) = cpu_to_be32( (uint32_t)(myNode->StartOffset) );

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
     * middle could leave us with two signed blocks, one with bogus data.
     * Break up the write into two parts: the data followed by the
     * signature.
     */
    activeBlockOffset = (backupBlock * getDyPartitionLength()) + sizeof(uint32_t);
    wrPtr = buff + sizeof(uint32_t);
    wrLen = bytesRead - sizeof(uint32_t);
    /* 
     * If there is no data to be copied, skip down to the signature copy
     */
    if( 0 != wrLen )
    {
        partErr = WritePartition(pHandle, wrPtr, activeBlockOffset, wrLen, &len);
        if ((partErr != PARTMGR_OK) || (len != wrLen))
        {
            /* Make sure backup block is still clean and exit */
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): unable to write data to backup partition\n", __LINE__);
            activeBlockOffset = backupBlock * getDyPartitionLength();
            ErasePartition(pHandle, activeBlockOffset, getDyPartitionLength());
            ASSERT(false);
            goto EXIT;
        }
    }

    /* Write our block signature to the backup block */
    activeBlockOffset = backupBlock * getDyPartitionLength();
    wrPtr = buff;
    wrLen = sizeof(uint32_t);
    partErr = WritePartition(pHandle, wrPtr, activeBlockOffset, wrLen, &len);
    if ((partErr != PARTMGR_OK) || (len != wrLen))
    {
        /* Make sure backup block is still clean and exit */
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): unable to write sig to backup partition\n", __LINE__);
        ErasePartition(pHandle, activeBlockOffset, getDyPartitionLength());
        ASSERT(false);
        goto EXIT;
    }

    /* Erase active block (data already copied to backup block) */
    activeBlockOffset = ActiveBlock * getDyPartitionLength();
    ErasePartition(pHandle, activeBlockOffset, getDyPartitionLength());

    /* Make the cloned list the master */
    DumpNodeList(VarBlockListSPI);
    MEM_FREE_AND_NULL(VarBlockListSPI);
    VarBlockListSPI = newBlockList;
    newBlockList = NULL;

    /* Switch the active block */
    ActiveBlock = backupBlock;

    /* Update dynamic write pointer (relative to active block) */
    setDyWritePtrSPI(bytesRead);

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SPI, "NV_DEV_SPI(%d): Switched active block to %d on %s\n", 
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
static uint32_t GetCommittedSizeSPI(ATLISTENTRY *theList)
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
static uint32_t getDyWritePtrSPI(void)
{
    return DyWriteOffsetSPI;
}


/**
 *
 *  \brief Sets pointer to next dynamic write position
 *
 *  \param [in] WriteOffset
 *
 *  \return error_type_t
 **/
static error_type_t setDyWritePtrSPI(uint32_t WriteOffset)
{
    /* Word align write pointer */
    if (WriteOffset & 0x00000003)
    {
        WriteOffset += 4;
        WriteOffset &= 0xFFFFFFFC;
    }
            
    DyWriteOffsetSPI = WriteOffset;
            
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
  
    cmd_printf( "Active block=%lu Write Ptr=0x%lx Max Partition Length=0x%lx\n\n",
                  ActiveBlock, getDyWritePtrSPI(), getDyPartitionLength() );
  
    searchNode = blkList->nextEntry;
    if ( searchNode == blkList )
    {
        cmd_printf("Partition Table is Empty\n");
        return;
    }

    while (searchNode != blkList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if (myNode != NULL)
        {
            cmd_printf( "%s Block :  ", getDevIdStr(myNode->MemDev) );
            cmd_printf("'%c%c%c%c'", myNode->Label[0],myNode->Label[1],
                       myNode->Label[2],myNode->Label[3] );
            cmd_printf( " offset=0x%lx, block size=0x%lx\n", 
                        (uint32_t)myNode->StartOffset, (uint32_t)myNode->Size );
        }
        searchNode = searchNode->nextEntry;
    }
}
  
static const char *nvspi_cmd_list_desc = "Lists current variable blocks on the nvram SPI part";
static const char *nvspi_cmd_list_usage = NULL;
static const char *nvspi_cmd_list_notes = NULL;
static const char *nvspi_cmd_clr_desc = "Clears the list of current variable blocks from the nvram SPI part";
static const char *nvspi_cmd_clr_usage = NULL;
static const char *nvspi_cmd_clr_notes = NULL;
static const char *nvspi_cmd_swap_desc = "switches to the backup block on the nvram SPI part";
static const char *nvspi_cmd_swap_usage = NULL;
static const char *nvspi_cmd_swap_notes = NULL;

static int nvspi_cmd_list_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    ListBlocks(VarBlockListSPI);

    return CMD_OK;
}

static int nvspi_cmd_clr_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    DumpNodeList(VarBlockListSPI);

    return CMD_OK;
}

static int nvspi_cmd_swap_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    SwitchToBackupBlock(MEM_SPI0);        

    return CMD_OK;
}

#endif

