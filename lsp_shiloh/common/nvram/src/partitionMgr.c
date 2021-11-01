/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file partitionMgr.c
 *
 * \brief Partition Manager Implementation
 *
 * This file implements the partition manager API, please see the 
 * partitionMgrAPI.h file for detailed interface descriptions.
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "error_types.h"
#include "list.h"
#include "memAPI.h"
#include "debug.h"
#include "dprintf.h"
#include "nvram_logger.h"
#include "lassert.h"

#include "nvram_config.h"
#include "deviceid.h"
#include "memdevice.h"
#include "partitionMgr.h"
#include "partitionMgrAPI.h"
#include "nodeCache.h"
#include "imageMapUtil.h"
#include "rommgrapi.h"
#include "utils.h"
#include <pthread.h>

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

/**
 *  \brief Cached device info struct
 *  
 *  We cache info for each installed device to speed up read and write operations: 
 *  otherwise we would have to locate the code table offset (and other data) for 
 *  each operation.
 **/
typedef struct dev_cache_s
{
    MEMORY_DEVICE    Dev;               /**< Enumerated memory device          */
    bool             validDev;          /**< Is device valid?                  */
    DRIVER_CONFIG_t  devInfo;           /**< Device information                */
    ImageMapInfo_t   imInfo;            /**< Image map details                 */
        
    ATLISTENTRY      listNode;          /**< For the linked list pointers      */
} dev_cache_t;


/*
 * Local (static) Variables
 */
static ATLISTENTRY StPartitionList;     /**< static partition table (all Parts) */
static ATLISTENTRY DevCacheList;        /**< cached device info list            */


/*
 * Local (static) function prototypes
 */ 
static dev_cache_t    *FindDevCache(MEMORY_DEVICE memDev); 
static void            CreateDevCache(MEMORY_DEVICE memDev);
static void            UpdatePartitionTable(MEMORY_DEVICE  memDev);


#ifdef HAVE_CMD
static int partitionmgr_cmd_list_cb( int argc, char *argv[] );
static int partitionmgr_cmd_read_cb( int argc, char *argv[] );
static int partitionmgr_cmd_debug_set_cb( int argc, char *argv[] );
static int partitionmgr_cmd_debug_clear_cb( int argc, char *argv[] );
static void ListPartition(ATLISTENTRY *PartList);

static const char *partitionmgr_cmd_list_desc;
static const char *partitionmgr_cmd_list_usage;
static const char *partitionmgr_cmd_list_notes;
static const char *partitionmgr_cmd_read_desc;
static const char *partitionmgr_cmd_read_usage;
static const char *partitionmgr_cmd_read_notes;
static const char *partitionmgr_cmd_debug_set_desc;
static const char *partitionmgr_cmd_debug_set_usage;
static const char *partitionmgr_cmd_debug_set_notes;
static const char *partitionmgr_cmd_debug_clear_desc;
static const char *partitionmgr_cmd_debug_clear_usage;
static const char *partitionmgr_cmd_debug_clear_notes;

#endif


/*
 * FUNCTION NAME: InitPartitionManager 
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
void InitPartitionManager(void)
{
    uint32_t  dev;
#ifdef HAVE_CMD
    int cmd_res;
#endif
    
    DBG_PRINTF(LOG_DEBUG|DBG_PARTMGR, "\nPARTMGR(%d): START InitPartitionManager (static partition scan)\n",
                __LINE__);

    /* Initialize partition table list */
    memset(&StPartitionList,  0, sizeof(ATLISTENTRY));
    ATInitList(&StPartitionList);

    /* Initialize cached device info list */
    memset(&DevCacheList,  0, sizeof(ATLISTENTRY));
    ATInitList(&DevCacheList);

    /* Open all the memory devices, read the code table and initialize the partition
     * table with all the available partitions. Klockwork may get upset with this ...
     */
    for (dev = MEM_I2C; dev < MEM_DEVICE_NOT_FOUND; dev++)
    {
        if (memDevInstalled(dev))
        {
            /* Cache device information (must be done before any other device access) */
            CreateDevCache(dev);
            
            /* Scan for partitions and create a RAM based directory */
            UpdatePartitionTable(dev);
        }
        else
        {
            DBG_PRINTF(LOG_DEBUG|DBG_PARTMGR, "PARTMGR(%d): %s not installed/registered\n", __LINE__, getDevIdStr(dev));
        }
    }
     
#ifdef HAVE_CMD
    cmd_res = cmd_register_cmd( "partmgr",
                                NULL,
                                "partition manager-related commands",
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "partmgr",
                                   "list",
                                   partitionmgr_cmd_list_desc,
                                   partitionmgr_cmd_list_usage,
                                   partitionmgr_cmd_list_notes,
                                   partitionmgr_cmd_list_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "partmgr",
                                   "read",
                                   partitionmgr_cmd_read_desc,
                                   partitionmgr_cmd_read_usage,
                                   partitionmgr_cmd_read_notes,
                                   partitionmgr_cmd_read_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "partmgr",
                                   "debug",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "partmgr debug",
                                   "set",
                                   partitionmgr_cmd_debug_set_desc,
                                   partitionmgr_cmd_debug_set_usage,
                                   partitionmgr_cmd_debug_set_notes,
                                   partitionmgr_cmd_debug_set_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "partmgr debug",
                                   "clear",
                                   partitionmgr_cmd_debug_clear_desc,
                                   partitionmgr_cmd_debug_clear_usage,
                                   partitionmgr_cmd_debug_clear_notes,
                                   partitionmgr_cmd_debug_clear_cb );
    ASSERT( CMD_OK == cmd_res );
#endif

    DBG_PRINTF(LOG_DEBUG|DBG_PARTMGR, "PARTMGR(%d): FINISH InitPartitionManager\n\n", __LINE__);
}


/* FUNCTION NAME: SearchForPartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
MEMORY_DEVICE SearchForPartition(uint32_t              ID,
                                 ImageMapType_t        Type,
                                 const MEMORY_DEVICE  *searchList)
{
    MEMORY_DEVICE  retDev = MEM_DEVICE_NOT_FOUND;
    uint32_t       i      = 0;
    BLOCK_NODE_t  *myNode;

    if (searchList == NULL)
    {
        ASSERT(false);
        return retDev;
    }    

    /* Run through the search list and return the first device that contains the
     * partition we are looking for
     */
    while (MEM_DEVICE_NOT_FOUND != searchList[i])
    {
        myNode = FindNodeByIdTypeDev(ID, Type, searchList[i], &StPartitionList);
        if (myNode != NULL)
        {
            /* Log the find and break out of the search loop */
            retDev = searchList[i];
            break;
        }
        i++;
    }

    return retDev;
}


/* FUNCTION NAME: GetPartitionInfo
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t GetPartitionInfo(uint32_t           ID,
                                 ImageMapType_t     Type,
                                 MEMORY_DEVICE      Dev,
                                 partition_info_t  *Info)
{
    BLOCK_NODE_t    *myNode;

    if (Info == NULL)
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Find the partition entry in the static partition list */
    myNode = FindNodeByIdTypeDev(ID, Type, Dev, &StPartitionList);
    if (myNode == NULL)
    {
        DBG_PRINTF(LOG_INFO|DBG_PARTMGR, "PARTMGR(%d): ID=0x%x Type=%d not found on %s\n", 
                   __LINE__, ID, Type, getDevIdStr(Dev));
        return PARTMGR_NOTFOUND;
    }

    /* Return information on the located partition */
    Info->Dev         = myNode->MemDev;
    Info->StartOffset = myNode->StartOffset;
    Info->Size        = myNode->Size;
    Info->Flags       = myNode->Flags;
    Info->Version     = myNode->Version;

    return PARTMGR_OK;
}


/* FUNCTION NAME: GetPartitionInfoByHandle
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t GetPartitionInfoByHandle(partition_handle_t *Handle, 
                                         partition_info_t   *Info)
{
    if ((Handle == NULL) || (Info == NULL))
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Return information from provided handle */
    Info->Dev         = Handle->partition.MemDev;
    Info->StartOffset = Handle->partition.StartOffset;
    Info->Size        = Handle->partition.Size;
    Info->Flags       = Handle->partition.Flags;
    Info->Version     = Handle->partition.Version;

    return PARTMGR_OK;
}


/* FUNCTION NAME: GetDeviceInfo
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t GetDeviceInfo(MEMORY_DEVICE memDev, 
                              uint32_t     *blockCount, 
                              uint32_t     *blockSize,
                              uint32_t     *pageSize)
{
    dev_cache_t  *myDev;

    if ((blockCount == NULL) || (blockSize == NULL) || (pageSize == NULL))
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Attempt to locate a cache entry for the passed memory device */
    myDev = FindDevCache(memDev);
    if (myDev == NULL)
    {
        return PARTMGR_NODEVICE;        
    }

    /* Device is registered, check if valid */
    if (myDev->validDev == true)
    {
        *blockCount = myDev->devInfo.BlockCount;
        *blockSize  = myDev->devInfo.BlockSize;
        *pageSize   = myDev->devInfo.PageSize;
    }
    else
    {
        *blockCount = 0;
        *blockSize  = 0;
        *pageSize   = 0;
        return PARTMGR_NODEVICE;
    }            

    return PARTMGR_OK;
}


/* FUNCTION NAME: OpenPartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t OpenPartition(uint32_t             ID,
                              ImageMapType_t       Type, 
                              MEMORY_DEVICE        Dev, 
                              partition_handle_t **Handle)
{
    BLOCK_NODE_t       *myNode;
    partition_handle_t *newHandle;

    if (Handle == NULL)
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }
    
    /* Set the return handle in case we fail */
    *Handle = NULL;
    
    /* Find the partition entry in the static partition list */
    myNode = FindNodeByIdTypeDev(ID, Type, Dev, &StPartitionList);
    if (myNode == NULL)
    {
        DBG_PRINTF(LOG_INFO|DBG_PARTMGR, "PARTMGR(%d): ID=0x%x Type=%d not found on %s\n", 
                     __LINE__, ID, Type, getDevIdStr(Dev));
        return PARTMGR_NOTFOUND;
    }
    
    /* Allocate the memory for the new partition handle */
    newHandle = (partition_handle_t *)MEM_MALLOC(sizeof(partition_handle_t));
    if (newHandle == NULL)
    {
        return PARTMGR_MEMERR;
    }
 
    /* Setup the new handle based on the cached node information */
    memset(newHandle, 0, sizeof(partition_handle_t));
    memcpy(&newHandle->partition, myNode, sizeof(BLOCK_NODE_t));

    /* Return the new handle to the caller.  Note that we did not open
     * the low level device driver at this point (memDevH will remain
     * NULL): going to try only opening when doing the first read/write/erase 
     * operation.
     */
    *Handle = newHandle;

    return PARTMGR_OK;
}


/* FUNCTION NAME: ClosePartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t ClosePartition(partition_handle_t *Handle)
{
    if (Handle == NULL)
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }
    
    /* Double check that the low level device driver is closed */
    if (Handle->memDevH != NULL)
    {
        devClose(Handle->memDevH);
        Handle->memDevH = NULL;
    }
    
    MEM_FREE_AND_NULL(Handle);
    return PARTMGR_OK;
}


/* FUNCTION NAME: ReadPartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t ReadPartition(partition_handle_t *Handle, 
                              int8_t             *Dest, 
                              uint64_t            Address, 
                              uint32_t            Length,
                              uint32_t           *bytesRead)
{
    uint64_t      physAddress;
    int32_t       actBytesRead;
    dev_cache_t  *myDev;
    
    /* Verify parameters are sane */
    if ((Handle == NULL) || (Dest == NULL) || (bytesRead == NULL) || (Length == 0))
    {
        /* Trap the error for now */
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Initialize the return value in case we fail */
    *bytesRead = 0;

    /* Keep folks from reading off the end of the partition. The partition address
     * is relative to the partition.
     */
    if ( (Address + Length) > Handle->partition.Size )
    {
        return PARTMGR_LENERROR;
    }

    /* Attempt to locate a cached entry for our memory device */
    myDev = FindDevCache(Handle->partition.MemDev);
    if (myDev == NULL)
    {
        return PARTMGR_NODEVICE;        
    }

    /* Device is registered, check if valid */
    if ((myDev->validDev == false) || (myDev->imInfo.validImageMap == false))
    {
        return PARTMGR_DEVERROR;
    }
        
    /* The low level device driver may not be open yet */
    if (Handle->memDevH == NULL)
    {
        if (devOpen(Handle->partition.MemDev, &Handle->memDevH) != OK)
        {
            return PARTMGR_DEVERROR;
        }
    }

    /* Convert the partition relative address to a virtual device address:
     *   (partition relative address) +  
     *   (partition start offset, which is relative to the code table) + 
     *   (offset from code table to beginning of device)
     */
    physAddress = Address + Handle->partition.StartOffset + myDev->imInfo.imOffset;
    actBytesRead = devRead(Handle->memDevH, 
                           Dest, 
                           physAddress, 
                           Length);

    /* Nand ECC should NOT return as an Error - this will be Erased page */
    if ((Handle->partition.MemDev == MEM_NAND0) && (actBytesRead == -2))
    {
        actBytesRead = Length;  
    }

    /* Going to try keeping the low level driver open as little as
     * possible, so close it now.  The partition close will catch it
     * if we don't do it here.
     */
    if (devClose(Handle->memDevH) == OK)
    {
        Handle->memDevH = NULL;
    }

    /* Check read status, bail if size mismatch */
    if (actBytesRead != Length)
    {
        return PARTMGR_DEVERROR;
    }

    *bytesRead = actBytesRead;
    return PARTMGR_OK;    
}


/* FUNCTION NAME: WritePartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t WritePartition(partition_handle_t *Handle, 
                               int8_t             *Src, 
                               uint64_t            Address, 
                               uint32_t            Length,
                               uint32_t           *bytesWritten)
{
    uint64_t      physAddress;
    int32_t       actBytesWritten;
    dev_cache_t  *myDev;
    
    /* Verify parameters are sane */
    if ((Handle == NULL) || (Src == NULL) || (bytesWritten == NULL) || (Length == 0))
    {
        /* Trap the error for now */
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Initialize the return value in case we fail */
    *bytesWritten = 0;

    /* Keep folks from writing off the end of the partition. The partition address
     * is relative to the partition.
     */
    if ( (Address + Length) > Handle->partition.Size )
    {
        return PARTMGR_LENERROR;
    }

    /* Attempt to locate a cached entry for our memory device */
    myDev = FindDevCache(Handle->partition.MemDev);
    if (myDev == NULL)
    {
        return PARTMGR_NODEVICE;        
    }

    /* Device is registered, check if valid */
    if ((myDev->validDev == false) || (myDev->imInfo.validImageMap == false))
    {
        return PARTMGR_DEVERROR;
    }
        
    /* The low level device driver may not be open yet */
    if (Handle->memDevH == NULL)
    {
        if (devOpen(Handle->partition.MemDev, &Handle->memDevH) != OK)
        {
            return PARTMGR_DEVERROR;
        }
    }

    /* Convert the partition relative address to a virtual device address:
     *   (partition relative address) +  
     *   (partition start offset, which is relative to the code table) + 
     *   (offset from code table to beginning of device)
     */
    physAddress = Address + Handle->partition.StartOffset + myDev->imInfo.imOffset;
    actBytesWritten = devWrite(Handle->memDevH, 
                               Src, 
                               physAddress, 
                               Length);

    /* Going to try keeping the low level driver open as little as
     * possible, so close it now.  The partition close will catch it
     * if we don't do it here.
     */
    if (devClose(Handle->memDevH) == OK)
    {
        Handle->memDevH = NULL;
    }

    /* Check read status, bail if size mismatch */
    if (actBytesWritten != Length)
    {
        return PARTMGR_DEVERROR;
    }

    *bytesWritten = actBytesWritten;
    return PARTMGR_OK;    
}


/* FUNCTION NAME: ErasePartition
 *
 * Please see partitionMgrAPI.h for function documentation.
 */
PARTMGR_ERROR_t ErasePartition(partition_handle_t *Handle, 
                               uint64_t            Address, 
                               uint32_t            Length)
{
    uint32_t         eraseSector;
    uint32_t         numSectors;
    uint64_t         physAddress;
    dev_cache_t     *myDev;
    int8_t         *firstSector=NULL, *lastSector=NULL;
    uint32_t    firstBlockAddress = 0;
    uint32_t    lastBlockAddress = 0;
    
    /* Verify parameters are sane */
    if ((Handle == NULL) || (Length == 0))
    {
        ASSERT(false);
        return PARTMGR_PARMERR;
    }

    /* Attempt to locate a cached entry for our memory device */
    myDev = FindDevCache(Handle->partition.MemDev);
    if (myDev == NULL)
    {
        return PARTMGR_NODEVICE;        
    }

    /* Device is registered, check if valid */
    if ((myDev->validDev == false) || (myDev->imInfo.validImageMap == false))
    {
        return PARTMGR_DEVERROR;
    }

    /* Keep folks from erasing past the end of the logical partition. The 
     * partition address is relative to the partition start.
     */
    if ( (Address + Length) > Handle->partition.Size )
    {
        return PARTMGR_LENERROR;
    }

    /* The device erase command tends to erase individual sectors (blocks), so
     * we need to look at the passed parameters and figure out what to do here.
     * For now force the caller to properly align the address and length
     * parameters.
     *
     * Convert the partition relative address to a virtual device address:
     *   (partition relative address) +  
     *   (partition start offset, which is relative to the code table) + 
     *   (offset from code table to beginning of device)
     */
    physAddress = Address + Handle->partition.StartOffset + myDev->imInfo.imOffset;

    /* The low level device driver may not be open yet */
    if (Handle->memDevH == NULL)
    {
        if (devOpen(Handle->partition.MemDev, &Handle->memDevH) != OK)
        {
            return PARTMGR_DEVERROR;
        }
    }
        
    /* Figure the number of the sector to erase */
    eraseSector = physAddress / myDev->devInfo.BlockSize;
    //
    // See if this is a partial erase.  In that case, erase the block and
    // write the stuff that needs to remain.  NOTE:  This will not work with nand
    // this should be moved to a part specific part of the code so that each
    // part can be handled correctly.  For eeprom and flash this works fine.
    //

    if(physAddress % myDev->devInfo.BlockSize)
    {
        // since we are writing part way into a block,
        // get a copy of the first sector so we don't corrupt anything.
        // and we can write the non-erased parts later.
        //
        firstSector = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), myDev->devInfo.BlockSize);
        firstBlockAddress = eraseSector*myDev->devInfo.BlockSize;
        devRead(Handle->memDevH, firstSector,firstBlockAddress,myDev->devInfo.BlockSize);
    }
    // check the end of the allocation to see if it lines up on a block boundry.
    //
    if((physAddress + Length) % myDev->devInfo.BlockSize) // 
    {
        // figure the address of this last block.
        lastBlockAddress = ((physAddress + Length)/ myDev->devInfo.BlockSize)*myDev->devInfo.BlockSize;
        // the last address is not aligned on a block boundry, get a copy of the last sector
        // for writing later.
        //
        if(firstSector && (eraseSector == (physAddress + Length)/myDev->devInfo.BlockSize))
        {
            // we already have a copy of the block, we are done.
            lastSector = firstSector;
        } else
        {
            lastSector = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), myDev->devInfo.BlockSize);
            devRead(Handle->memDevH, lastSector, 
                    // get the address of the block that contains the last sector.
                    lastBlockAddress,
                    myDev->devInfo.BlockSize);
        }
    }
    numSectors  = Length / myDev->devInfo.BlockSize;
    if(Length % myDev->devInfo.BlockSize)
        numSectors++;               // Make sure correct value.
    DBG_PRINTF(LOG_INFO|DBG_PARTMGR, "PARTMGR(%d): Erase start sector=%d numSectors=%d\n", 
                __LINE__, eraseSector, numSectors);

    /* Loop around and erase the sectors */
    while (numSectors)
    {
        devErase(Handle->memDevH, eraseSector);
        
        numSectors--;
        eraseSector++;
    }        
    //
    // If we didn't line up on block boundries, write the data on either end of the
    // erase back to the block.
    //
    if(firstSector) // do we have something before the erased block to put back?
    {
        // figure the address of the start of the block
        devWrite(Handle->memDevH, firstSector,firstBlockAddress,physAddress - firstBlockAddress);
        // since firstsector and lastSector can point to the same buffer, only free if
        // they are different.
        //
        if(firstSector != lastSector)
            memFree(firstSector);
    }
    if(lastSector) // do we have anything after the erased block to write?
    {
        // Figure the offset to get to the start address of the data in the block.
        firstBlockAddress = physAddress + Length - lastBlockAddress;
        devWrite(Handle->memDevH, lastSector + firstBlockAddress, physAddress + Length,
                 lastBlockAddress + myDev->devInfo.BlockSize-physAddress - Length);
        memFree(lastSector);
    }
    //
    // only the section requested is erased, we are done.
    //
    /* Going to try keeping the low level driver open as little as
     * possible, so close it now.  The partition close will catch it
     * if we don't do it here.
     */
    if (OK == devClose(Handle->memDevH))
    {
        Handle->memDevH = NULL;
    }

    return PARTMGR_OK;    
}


/**
 *  \brief Builds a RAM partition list for the specified memory device
 *
 *  Reads the code table entries from the physical device and updates
 *  our internal RAM based partition table.  This  function should be 
 *  called once at boot for each physical device.
 *
 *  \param[in] memDev   Memory device to scan for partitions
 *
 **/
static void UpdatePartitionTable(MEMORY_DEVICE  memDev)
{
    uint32_t         i;
    int8_t          *Buffer;
    BLOCK_NODE_t    *partEntry;
    MEMDEV_HANDLE   *devHandle;
    dev_cache_t     *myDev;
    uint32_t         imageMapSize;
    uint64_t         imAddr;
    
    /* Attempt to locate a cache entry for the passed memory device */
    myDev = FindDevCache(memDev);
    if (myDev == NULL)
    {
        return;        
    }

    /* Verify valid device and code table. Also make sure we have entries to process */
    if ( (myDev->validDev == false) || 
         (myDev->imInfo.validImageMap == false) ||
         (myDev->imInfo.imNumEntries == 0) )
    {
        return;
    }

    /* Allocate a buffer to store the entire code table */
    imageMapSize = myDev->imInfo.imLenBytes;
    Buffer = MEM_MALLOC_ALIGN(imageMapSize, cpu_get_dcache_line_size());
    if (Buffer == NULL)
    {
        return;
    }

    /* Open up the memory device */
    if (OK == devOpen(memDev, &devHandle))
    {
        imAddr = myDev->imInfo.imOffset;
        if (devRead(devHandle, Buffer, imAddr, imageMapSize) != imageMapSize)
        {
            DBG_PRINTF(LOG_ERR|DBG_PARTMGR, "PARTMGR(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
            devClose(devHandle);
            MEM_FREE_AND_NULL(Buffer);
            return;
        }
        
        devClose(devHandle);
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_PARTMGR, "PARTMGR(%d): ERROR can't open %s\n", __LINE__, getDevIdStr(memDev));
        MEM_FREE_AND_NULL(Buffer);
        return;        
    }     

    /* Parse through the code table and populate the partition list */    
    for (i = 0; i < myDev->imInfo.imNumEntries; i++)
    {
        partEntry = AllocNewNode();
        if (NULL == partEntry)
        {
            ASSERT(false);    
            MEM_FREE_AND_NULL(Buffer);
            return;        
        }
         
        if (OK == ReadImageMapEntry(&myDev->imInfo, &myDev->devInfo, Buffer, i, partEntry))
        {
            /* Insert the partition in the partition list. Note that this list stores
             * partitions for all parts, no check is done for duplicates.
             */
            DBG_PRINTF(LOG_INFO|DBG_PARTMGR, "PARTMGR(%d): add ID=0x%x Type=%d offset=0x%x len=0x%x on %s\n", 
                         __LINE__, partEntry->ID, partEntry->Type, (uint32_t)partEntry->StartOffset, 
                         (uint32_t)partEntry->Size, getDevIdStr(partEntry->MemDev) );
                    
            ATInsertTailList(&StPartitionList, &partEntry->listNode);
        }
        else
        {
            MEM_FREE_AND_NULL(partEntry);
        }
    }

   MEM_FREE_AND_NULL(Buffer);
}


/**
 *  \brief 
 *
 **/
static dev_cache_t *FindDevCache(MEMORY_DEVICE memDev)
{
    ATLISTENTRY  *searchNode;
    dev_cache_t  *myNode;
 
    /* Put searchNode at the first entry after the start (it will be the head
     * node if the list is empty).
     */
    searchNode = DevCacheList.nextEntry;      

    /* Search until we find our node or we hit the head node */
    while (searchNode != &DevCacheList)
    {
        myNode = CONTAINING_RECORD(searchNode, dev_cache_t, listNode);
        if ((myNode != NULL) && (myNode->Dev == memDev))
        {
            return myNode;
        }
        searchNode = searchNode->nextEntry;
    }
    
    /* Didn't find a match */
    return NULL;    
}


/**
 *  \brief 
 *
 **/
static void CreateDevCache(MEMORY_DEVICE memDev)
{
    dev_cache_t     *myNode;
    MEMDEV_HANDLE   *devHandle;

    /* See if we already have a node for this device */
    myNode = FindDevCache(memDev);
    if (myNode != NULL)
    {
        /* Remove it from the list while we update it */
        ATRemoveEntryList(&myNode->listNode);
    }
    else
    {
        /* Need to allocate a new node */
        myNode = (dev_cache_t *) MEM_MALLOC(sizeof(dev_cache_t));
        if (myNode == NULL)
        {
            return;
        }
    }

    /* Clear out the data for this device cache entry */
    memset(myNode,  0, sizeof(dev_cache_t));
    myNode->Dev      = memDev;
    myNode->validDev = false;
    myNode->imInfo.validImageMap = false;
    
    /* Fill out the device information */
    if (memDevInstalled(memDev))
    {
        /* Go down to the actual device layer to get the basic device info */
        if (OK == devOpen(memDev, &devHandle))
        {
            if (OK == devReport(devHandle, &myNode->devInfo))
            {
                myNode->validDev   = true;
            }
            
            devClose(devHandle);    
        }

        /* Get image map (code table) information */
        if (myNode->validDev == true)
        {
            FindImageMap(memDev, &myNode->imInfo);
        }
    }
        
    /* All done, put it on the list */
    ATInsertTailList(&DevCacheList, &myNode->listNode);
}


#ifdef HAVE_CMD 

static const char *partitionmgr_cmd_list_desc = "Lists current static partitions";
static const char *partitionmgr_cmd_list_usage = NULL;
static const char *partitionmgr_cmd_list_notes = NULL;
static const char *partitionmgr_cmd_read_desc = "Read the data partition and return the data";
static const char *partitionmgr_cmd_read_usage = "<type> <id>";
static const char *partitionmgr_cmd_read_notes = "type is a numerical representation of the enum ImageMapType_t.  id is a partition manager data partition id, use the list command to see the data partitions.  Note that this command appends the size of the partition it read as a cmd result";
static const char *partitionmgr_cmd_debug_set_desc = "Set the debug logging levels for this module";
static const char *partitionmgr_cmd_debug_set_usage = "<level 1> [<level2>] ... <submodule 1> [<submodule 2>] ...";
static const char *partitionmgr_cmd_debug_set_notes = "level is one or more of the following: alert, critical, error, warning, notice, info, debug - in order"
                                               " if you specify warning you also get the ones listed before. "
                                               "submodule is one or more of the following: nvram partmgr dev_spi dev_shim dev_eeprom imagemap memdevice"
                                               "For example to enable alert and critical levels for imagemap and partmgr type: <cmd> alert critical imagemap partmgr";
static const char *partitionmgr_cmd_debug_clear_desc = "Disable all debug logging for this module";
static const char *partitionmgr_cmd_debug_clear_usage = NULL;
static const char *partitionmgr_cmd_debug_clear_notes = NULL;

static int partitionmgr_cmd_list_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_printf("Static Partition List:\n");
    ListPartition(&StPartitionList);

    return CMD_OK;
}

static int partitionmgr_cmd_read_cb( int argc, char *argv[] )
{    
    uint32_t id;
    uint32_t type;
    error_type_t err;
    int8_t *data;
    uint32_t ImageSize;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }

    err = str_mkint(argv[1], &type);
    if(err != OK)
    {
        cmd_printf("bad type specified \"%s\"\n", argv[1]);
        return CMD_USAGE_ERROR;
    }
    err = str_mkint(argv[2], &id);
    if(err != OK)
    {
        cmd_printf("bad id specified \"%s\"\n", argv[2]);
        return CMD_USAGE_ERROR;
    }

    data = RomMgrReadData( (ImageMapType_t)type, id, &ImageSize);
    if(data == NULL)
    {
        cmd_printf( "Failed to read partition with type %lx and id %lx (can't determine if this is a usage error -- please recheck arguments)\n",
                    type,
                    id );
        return CMD_ERROR;
    }

    memFree(data);

    cmd_append_result( "%lu", ImageSize );

    return OK;
}

static int partitionmgr_cmd_debug_clear_cb( int argc, char *argv[] )
{    
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    logger_clear_module( DEBUG_LOGGER_MODULE_NVRAM );

    return CMD_OK;
}

static int partitionmgr_cmd_debug_set_cb( int argc, char *argv[] )
{    
    int32_t pos;
    uint32_t temp;

    if ( argc < 3 )
    {
        return CMD_USAGE_ERROR;
    }

    pos = 1;
    temp = 0;

    while(pos < argc)
    {
        if(strncasecmp(argv[pos], "alert",5) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_ALERT;
        }
        else if (strncasecmp(argv[pos], "critical", 8) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_CRIT;
        }
        else if (strncasecmp(argv[pos], "error", 5) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_ERR;
        }
        else if (strncasecmp(argv[pos], "warning", 7) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_WARNING;
        }
        else if (strncasecmp(argv[pos], "notice", 6) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_NOTICE;
        }
        else if (strncasecmp(argv[pos], "info", 4) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_INFO;
        }
        else if (strncasecmp(argv[pos], "debug", 5) == 0)
        {
            temp = (temp & ~LOG_MAX_FLAG) | LOG_DEBUG;
        }
        else if (strncasecmp(argv[pos], "partmgr", 7) == 0)
        {
            temp |= DBG_PARTMGR;
        }
        else if (strncasecmp(argv[pos], "nvram", 5) == 0)
        {
            temp |= DBG_NVRAM;
        }
        else if (strncasecmp(argv[pos], "dev_spi", 7) == 0)
        {
            temp |= DBG_NVRAM_DEV_SPI;
        }
        else if (strncasecmp(argv[pos], "dev_shim", 7) == 0)
        {
            temp |= DBG_NVRAM_DEV_SHIM;
        }
        else if (strncasecmp(argv[pos], "dev_eeprom", 7) == 0)
        {
            temp |= DBG_NVRAM_DEV_EEPROM;
        }
        else if (strncasecmp(argv[pos], "imagemap", 7) == 0)
        {
            temp |= DBG_IMAGEMAP;
        }
        else if (strncasecmp(argv[pos], "memdevice", 7) == 0)
        {
            temp |= DBG_MEMDEVICE;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }

        pos++;
    }

    logger_enable_module_mask( DEBUG_LOGGER_MODULE_NVRAM, temp );

    return CMD_OK;
}


static void ListPartition(ATLISTENTRY *PartList)
{
    BLOCK_NODE_t *myNode;
    ATLISTENTRY  *searchNode;

    searchNode = PartList;
    if( searchNode->nextEntry == searchNode )
    {
        cmd_printf( "Partition Table is Empty\n" );
        return;
    }

    /* we have at least one entry in the table! */
    do
    {
        searchNode = searchNode->nextEntry;
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if (myNode != NULL)
        {
            cmd_printf( "%s Partition: ", getDevIdStr(myNode->MemDev) );
            cmd_printf( "ID=0x%04lX, Type=%lu ", myNode->ID, myNode->Type );
            cmd_printf( "Start Block=%lu, Block Count=%lu\n", myNode->StartBlock, myNode->BlockCount );
        }
    } while (searchNode->nextEntry != PartList);
}

#endif /* HAVE_CMD */
