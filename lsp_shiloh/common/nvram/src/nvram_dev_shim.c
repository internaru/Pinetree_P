/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_dev_shim.c
 *
 *  \brief NVRAM device shim
 *
 *  This file hides device specific knowledge from the NVRAM API layer.  Things
 *  that may differ for each memory device type:
 *   - NVRAM variable block storage algorithm
 *   - wear leveling requirements
 *   - available storage size
 *
 **/

#include <string.h>
#include <stdint.h>
#include "error_types.h"
#include "memAPI.h"
#include "list.h"
#include "lassert.h"
#include "checksum.h"
#include "deviceid.h"
#include "nvram_api.h"
#include "partitionMgrAPI.h"
#include "platform.h"
#include "nvram_dev_shim.h"
#include "nvram_dev_eeprom.h"
#include "nvram_dev_spi.h"
#include "nvram_dev_nand.h"
#include "nvram_config.h"
#include "nvram_logger.h"
#include "ioutils.h"

#ifdef IMAGEMAP_VER_1_0
#include "code_table.h"
#endif

#include "memdevice.h"    /* for HackSPI0CodeTable() */
#ifdef IMAGEMAP_VER_1_0
#include "hwconfig_api.h" /* for HackSPI0CodeTable() */
#endif

/*
 *  Local (static) variables
 */
static MEMORY_DEVICE validatedDev = MEM_DEVICE_NOT_FOUND;


/*
 *  Local (static) function prototypes
 */
static error_type_t  DiscoverVarBlocks(MEMORY_DEVICE memDev);
static error_type_t  ValidateContainer(MEMORY_DEVICE memDev);
static MEMORY_DEVICE FindFirstDeviceOnList(const MEMORY_DEVICE *searchList);

error_type_t  HackSPI0CodeTable(void);

/*
 * FUNCTION NAME: InitNvramDevShims
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t InitNvramDevShim(void)
{
    const MEMORY_DEVICE *searchList;
    MEMORY_DEVICE        targetDev;
    
    /* Only run through this logic once (at boot time) */
    if (validatedDev != MEM_DEVICE_NOT_FOUND)
    {
        /* We are already bound to a device */
        return OK;
    }

    /* Initialize some lower layers */
    InitMemDeviceAPI();
    InitPartitionManager();
    
    /* Get the device search list from the configuration layer */
    searchList = getNvStoreSearchList();

    /* Attempt to find our container partition on the search path */
    targetDev = SearchForPartition(VARBLOCK_PART_ID, IM_NV_STORE, searchList);
    if (MEM_DEVICE_NOT_FOUND == targetDev)
    {
        /* No existing container (which is usually OK), so we must now select
         * the highest priority device from the search list that is alive
         * in our system.
         */
        targetDev = FindFirstDeviceOnList(searchList);
    }
    
    /* Set up the shim layer for our target memory device */     
    switch (targetDev)
    {
        case MEM_I2C:
            /* Initialize the EEPROM (I2C) Shim layer */
            InitNvramEEPROM();
            break;
            
        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            /* Initialize the SPI Shim layer */
            InitNvramSPI();
            break;
            
        case MEM_NAND0:
            /* Initialize the NAND Shim layer */
            InitNvramNAND();
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): Invalid memory device\n", __LINE__);
            return FAIL;
            ASSERT(false);
            break;
    }

    /* All NVRAM variable blocks are stored in a physical device partition; a
     * 'container' partition which is created and managed on the device by the 
     * partition manager.  The storage format within that partition is handled
     * by the nvram device specific shim layer. Make sure our container partition
     * exists on the target device, create one if needed.
     */
    if (ValidateContainer(targetDev) != OK)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): problem with container partition on %s\n", 
                     __LINE__, getDevIdStr(targetDev));
        return FAIL;
    }
    
    return OK;
} 


/*
 * FUNCTION NAME: LocateVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t LocateVarBlock(nvram_handle_t *Handle)
{
    uint32_t      varBlockSize = 0;

    /* Fail if bad handle or no device validated */
    if ((Handle == NULL) || (validatedDev == MEM_DEVICE_NOT_FOUND))
    {
        return FAIL;
    }

    /* Note: only look on our validated device */
    switch (validatedDev)
    {
        case MEM_I2C:
            varBlockSize = GetVarBlockSizeEEPROM(validatedDev, Handle->BlockLabel);
            break;
            
        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            varBlockSize = GetVarBlockSizeSPI(validatedDev, Handle->BlockLabel);
            break;
            
        case MEM_NAND0:
            varBlockSize = GetVarBlockSizeNAND(validatedDev, Handle->BlockLabel);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d):LocateVarBlock Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    if (varBlockSize == 0)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): LocateVarBlock '%4s' on %s, not found\n", 
                    __LINE__, Handle->BlockLabel, getDevIdStr(validatedDev));
        return FAIL;
    }
    
    /* Return information on the located var block */
    Handle->DeviceID  = validatedDev;
    Handle->MaxLength = varBlockSize;

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): LocateVarBlock '%4s' found on %s with size=%d\n",
                    __LINE__, Handle->BlockLabel, getDevIdStr(Handle->DeviceID), Handle->MaxLength);

    return OK;
}


/*
 * FUNCTION NAME: RegisterVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t RegisterVarBlock(nvram_handle_t *Handle)
{
    error_type_t retStatus = FAIL;

    /* Validate input parameters */
    if ( (Handle == NULL) || (Handle->BlockLabel == NULL) || 
         (Handle->MaxLength == 0) || (validatedDev == MEM_DEVICE_NOT_FOUND) )
    {
        return FAIL;
    }

    /* Always use our validated memory device */
    Handle->DeviceID = validatedDev;

    /* Call the appropriate device shim to register the variable block */
    switch (Handle->DeviceID)
    {
        case MEM_I2C:
            retStatus = AddRmVarBlockEEPROM(Handle->BlockLabel, 
                                            Handle->DeviceID, 
                                            Handle->MaxLength, 
                                            ADD);
            break;

        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            retStatus = RegisterVarBlockSPI(Handle->BlockLabel,
                                            Handle->DeviceID, 
                                            Handle->MaxLength);
            break;
            
        case MEM_NAND0:
            retStatus = RegisterVarBlockNAND(Handle->BlockLabel,
                                            Handle->DeviceID, 
                                            Handle->MaxLength);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d):RegisterVarPartition Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    /* If we failed, disable the handle by putting in a bad device ID */
    if (retStatus != OK)
    {
        Handle->DeviceID = MEM_DEVICE_NOT_FOUND;
    }

    return retStatus;
}


/*
 * FUNCTION NAME: ReleaseVarBlock
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t ReleaseVarBlock(nvram_handle_t *Handle)
{
    error_type_t retStatus = FAIL;

    /* Validate input parameters */
    if ((Handle == NULL) || (Handle->BlockLabel == NULL) || (Handle->MaxLength == 0))
    {
        return retStatus;
    }

    /* Fail if no validated device or bogus device in handle */
    if ((validatedDev == MEM_DEVICE_NOT_FOUND) || (Handle->DeviceID != validatedDev))
    {
        return retStatus;
    }

    /* Call the appropriate device shim to delete the variable block */
    switch (Handle->DeviceID)
    {
        case MEM_I2C:
            retStatus = AddRmVarBlockEEPROM(Handle->BlockLabel,
                                            Handle->DeviceID, 0, REMOVE);
            break;

        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            retStatus = ReleaseVarBlockSPI(Handle->BlockLabel, Handle->DeviceID);
            break;

        case MEM_NAND0:
            retStatus = ReleaseVarBlockNAND(Handle->BlockLabel, Handle->DeviceID);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d):RegisterVarBlock Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    return retStatus;
}

/*
 * FUNCTION NAME: VarBlockRead
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockRead(nvram_handle_t *nHandle, int8_t *blockPtr, uint64_t offset, uint32_t size)
{
    uint32_t readCount = 0;
    
    if ((nHandle == NULL) || (blockPtr == NULL) || (size == 0))
    {
        return 0;
    }

    /* Fail if no validated device or bogus device in handle */
    if ((validatedDev == MEM_DEVICE_NOT_FOUND) || (nHandle->DeviceID != validatedDev))
    {
        return 0;
    }

    /* Call the appropriate device shim to read the variable block */
    switch (nHandle->DeviceID)
    {
        case MEM_I2C:
            readCount = VarBlockReadEEPROM(nHandle, blockPtr, offset, size);
            break;

        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            readCount = VarBlockReadSPI(nHandle, blockPtr, offset, size);
            break;
            
        case MEM_NAND0:
            readCount = VarBlockReadNAND(nHandle, blockPtr, offset, size);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d):VarBlockRead-Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    return readCount;  
}


/*
 * FUNCTION NAME: VarBlockWrite
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockWrite(nvram_handle_t *nHandle, int8_t *blockPtr)
{
    uint32_t writeCount = 0;

    if ((nHandle == NULL) || (nHandle->MaxLength == 0) || (blockPtr == NULL))
    {
        return 0;
    }

    /* Fail if no validated device or bogus device in handle */
    if ((validatedDev == MEM_DEVICE_NOT_FOUND) || (nHandle->DeviceID != validatedDev))
    {
        return 0;
    }

    /* Call the appropriate device shim to write the variable block */
    switch (nHandle->DeviceID)
    {
        case MEM_I2C:
            writeCount = VarBlockWriteEEPROM(nHandle, blockPtr);
            break;
            
        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            writeCount = VarBlockWriteSPI(nHandle, blockPtr);
            break;
            
        case MEM_NAND0:
            writeCount = VarBlockWriteNAND(nHandle, blockPtr);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d):VarBlockWrite-Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    return writeCount;     
}


/*
 * FUNCTION NAME: VarBlockCheckCRC
 *
 *   See nvram_dev_shim.h for function documentation.
 */
bool VarBlockCheckCRC(char * label, VARBLOCK_HEADER_t *varBlock, uint32_t erased_data_val)
{
    uint32_t  CRC;

    /* Quick parameter check */
    if ((varBlock == NULL) || (label == NULL))
    {
        ASSERT(false);
        return false;
    }
    
    /* REVISIT:
     * Our legacy data store comes in at the shim layer (bypassing nvram_api) and does
     * not get it's data wrapped with VARBLOCK_HEADER_t.  It uses its own header
     * type and does its own CRC checking.  Obsolete this ASAP.
     */
    if (memcmp("DSTR", label, 4) == 0)
    {
        /* Legacy data store, just return true */
        return true;
    }    
    
    // guard against running checksum on erased memory
    if(varBlock->CRC == erased_data_val && varBlock->Len == erased_data_val)
    {
	return false;
    }

    /* Calculate the checksum (including version) on the passed block */
    DBG_PRINTF(LOG_DEBUG, "VarBlockCheckCRC: block @ %#x, len = %d\n", &varBlock->Data, (sizeof(varBlock->Data) + varBlock->Len));
    CRC = CalcChecksum( (uint32_t *)&varBlock->Data, (sizeof(varBlock->Data) + varBlock->Len));
                                     
    if (CRC == varBlock->CRC)
    {
        return true;
    }
    
    return false;
}


/*
 * FUNCTION NAME: DiscoverVarBlocks
 *
 *   
 */
static error_type_t DiscoverVarBlocks(MEMORY_DEVICE memDev)
{
    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): START NV Var block discovery\n", __LINE__);

    switch (memDev)
    {
        case MEM_I2C:
            DiscoverVarBlocksEEPROM(memDev);
            break;

        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            DiscoverVarBlocksSPI(memDev);
            break;

        case MEM_NAND0:
            DiscoverVarBlocksNAND(memDev);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): Invalid memory device\n", __LINE__);
            ASSERT(false);
            break;
    }

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): FINISH NV Var block discovery\n\n", __LINE__);
    
    return OK;
}


/*
 * FUNCTION NAME: FindFirstDeviceOnList
 *
 *   
 */
static MEMORY_DEVICE FindFirstDeviceOnList(const MEMORY_DEVICE *searchList)
{
    MEMORY_DEVICE  retDev = MEM_DEVICE_NOT_FOUND;
    uint32_t       i      = 0;
    uint32_t       blockCount;
    uint32_t       blockSize;
    uint32_t       pageSize;
    
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
        if (PARTMGR_OK == GetDeviceInfo(searchList[i], &blockCount, &blockSize, &pageSize))
        {   
            /* Log the find and break out of the search loop */
            retDev = searchList[i];
            break;
        }
        i++;
    }

    return retDev;    
}


/*
 * FUNCTION NAME: ValidateContainer 
 *
 */
static error_type_t ValidateContainer(MEMORY_DEVICE memDev)
{
    error_type_t      retStatus = FAIL;
    PARTMGR_ERROR_t   existErr;
    partition_info_t  existInfo;
    uint32_t          newSize;
   
    /* Make sure we are passed a valid device */
    if (memDev == MEM_DEVICE_NOT_FOUND)
    {
        return FAIL;
    }
    
    /* See if we have already validated the container on this device */
    if (memDev == validatedDev)
    {
        /* Already done, save the work and exit now */
        return OK;
    }

    /*  Each device has it's own unique NVRAM variable block encoding, so the
     *  partition requirements tend to be different.  The device specific shim may
     *  also consult the configuration table to apply system level settings.
     */
    switch (memDev)
    {
        case MEM_I2C:
            GetPartitionNeedsEEPROM(memDev, &newSize);
            break;
            
        case MEM_SPI0:
        case MEM_SPI1:
        case MEM_SPI2:
        case MEM_SPI3:
        case MEM_SPI4:
            GetPartitionNeedsSPI(memDev, &newSize);
            break;
            
        case MEM_NAND0:
            GetPartitionNeedsNAND(memDev, &newSize);
            break;
        case MEM_NAND1:
        default:
            DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Invalid memory device\n", __LINE__);
            ASSERT(false);
            return retStatus;
            break;
    }

    /* Take a peek and see if the container partition already exists
     * on the specified device.  All containers currently use a partition
     * ID of VARBLOCK_PART_ID.
     */
    existErr = GetPartitionInfo(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &existInfo); 

    switch (existErr)
    {
        case PARTMGR_OK:
            /* The container partition was found, process existing var blocks if
             * the container is OK ... otherwise fall out with fail.
             */
            if (newSize <= existInfo.Size)
            {
                DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Container partition valid on %s\n", 
                        __LINE__, getDevIdStr(memDev));
                DiscoverVarBlocks(memDev);
                retStatus = OK;
            }
            break;
            
        case PARTMGR_NOTFOUND:           
            /* REVISIT:
             * Until our tools are ready, 'hack' a code table and partition into place 
             * at a fixed location.
             */
            if (memDev == MEM_SPI0)
            {
                retStatus = HackSPI0CodeTable();
            } 
            else
            {
                /* Our 'container' partition does not exist on the target device! */
                DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM,"NV_DEV_SHIM(%d): Container partition not found on %s\n", 
                        __LINE__, getDevIdStr(memDev));
            }
            break;
            
        case PARTMGR_NODEVICE:
            /* System configuration error: selected device not available */
            ASSERT(false);
            break;
            
        default:
            /* Partition not found and device error condition was raised. Just
             * fall out of this routine with a failure
             */
            break;
    }

    /* Mark the device as validated if we made it through OK */
    if (retStatus == OK)
    {
        validatedDev = memDev;
    }
    
    return retStatus;
}


/*
 * FUNCTION NAME: HackSPI0CodeTable 
 *
 */

error_type_t HackSPI0CodeTable(void)
{
    MEMDEV_HANDLE  *handle    = NULL;
    error_type_t    retStatus = FAIL;
    uint32_t       *blkBuff   = NULL;
    DRIVER_CONFIG_t devData;
    uint32_t        ctOffset;
    uint32_t        bytesWritten;
    uint32_t        bytesRead;

#ifdef IMAGEMAP_VER_1_0

    // no nvram table on spi.  Put on on there.
    //
    /* Open the low level device driver */
    if (devOpen(MEM_SPI0, &handle) != OK)
    {
        goto EXIT;
    }

    /* Get some stats on the SPI part */
    if (devReport(handle, &devData) != OK)
    {
        goto EXIT;
    }
    blkBuff = MEM_MALLOC_ALIGN(devData.BlockSize, cpu_get_dcache_line_size());
    if(blkBuff == NULL)
        goto EXIT;

    ctOffset  = 0;
    bytesRead = devRead(handle, (int8_t *)blkBuff, ctOffset, sizeof(code_table_header_t));
    if (bytesRead != sizeof(code_table_header_t))
    {
        goto EXIT;
    }
    if(((uint32_t *)blkBuff)[0] != 0xffffffff)
    {
       // put the table on block 1.
        ctOffset = devData.BlockSize;
    }
    memset(blkBuff, 0xff, devData.BlockSize);
    ct_build_header((uint32_t *)blkBuff, 1);   // build a header
    ct_add_entry(blkBuff, IM_NV_STORE, VARBLOCK_PART_ID, (devData.BlockCount-2)*devData.BlockSize-ctOffset, 2*devData.BlockSize);
    ct_complete_table(blkBuff);


    /* Make sure the taget block is erased */
    if (OK != devErase(handle, (ctOffset/devData.BlockSize)))
    {
        goto EXIT;
    }

    /* Lay down the updated block */
    bytesWritten = devWrite(handle, (int8_t *)blkBuff, ctOffset, devData.BlockSize);
    if (bytesWritten == devData.BlockSize)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Container partition created on MEM_SPI0: REBOOTING!\n", 
                     __LINE__);
        retStatus = OK;            
        hwPlatformReset();
    }

#endif

#ifdef IMAGEMAP_VER_0_0
    uint32_t        blkSig;
    uint32_t        nvStartOffset;
    uint32_t        wrOffset       = 1;
    uint32_t        smallestOffset = 0;
    
    /* HACK: Constants that match up to product's code table */
    const uint32_t  codeKey    = 0xC0DEED0C;   /* Code signature   */
    const uint32_t  ErasedData = 0xffffffff;   /* Erased SPI data  */
    const uint32_t  ctOffOff   = 0;            /* Offset to Offset */
    const uint32_t  ctOffLen   = 1;            /* Offset to length */
    const uint32_t  ctOffId    = 2;            /* Offset to ID     */
    const uint32_t  ctOffTerm  = 3;            /* Offset to term   */
    const uint32_t  ctLenWrds  = 3;            /* Entry length     */
    
    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Attempting to create container partition on MEM_SPI0\n", 
                  __LINE__);

    /* NOTE:
     * We got here because the NV Store container partition was no place to be found,
     * and the tools to create it don't exist yet.  This routine is a HACK and needs
     * to be removed soon as the 'image builder' tool is complete.
     *
     * Now we need to add a code table entry to the container partition, which can 
     * be in one of serveral states:
     *  - no existing code table: create one
     *  - existing table with space: add entry
     *  - existing table with no space: roll over and assert
     *
     * Be wary of boot code in block 0 (it's best not to corrupt it).
     */

    /* Open the low level device driver */
    if (devOpen(MEM_SPI0, &handle) != OK)
    {
        goto EXIT;
    }

    /* Get some stats on the SPI part */
    if (devReport(handle, &devData) != OK)
    {
        goto EXIT;
    }

    /* Calculate desired NV store location (at end of SPI part) */
    nvStartOffset = (devData.BlockCount - 2) * devData.BlockSize;

    /* Need to find a home for the code table: see if block 0 has boot code in it */
    ctOffset  = 0;
    bytesRead = devRead(handle, (int8_t *)&blkSig, ctOffset, sizeof(uint32_t));
    if (bytesRead != sizeof(uint32_t))
    {
        goto EXIT;
    }
    blkSig = be32_to_cpu(blkSig);
    if ((blkSig != codeKey) && (blkSig != ErasedData))
    {
        /* Code table must be on block 1 */
        ctOffset  = devData.BlockSize;
        bytesRead = devRead(handle, (int8_t *)&blkSig, ctOffset, sizeof(uint32_t));
        if (bytesRead != sizeof(uint32_t))
        {
            goto EXIT;
        }
        blkSig = be32_to_cpu(blkSig);
    }

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): ctOffset=0x%x blkSig=0x%x nvStartOffset=0x%x\n", 
                __LINE__, ctOffset, blkSig, nvStartOffset);

    /* Allocate enough working RAM to store an entire block of SPI: we may need
     * to save some of the contents when the block gets erased.
     */
    blkBuff = (uint32_t *)MEM_MALLOC(devData.BlockSize);
    if (blkBuff == NULL)
    {
        goto EXIT;
    }

    /* See if we have an existing code table */
    if (blkSig != codeKey)
    {
        /* No existing table: Clear out our memory buffer (erase it) */
        memset((int8_t *)blkBuff, 0xff, devData.BlockSize);
        
        /* Sign our new block buffer */
        blkBuff[0] = cpu_to_be32(codeKey);
    }
    else
    {
        /* Existing code table, read entire block into memory */
        bytesRead = devRead(handle, (int8_t *)blkBuff, ctOffset, devData.BlockSize);
        if (bytesRead != devData.BlockSize)
        {
            goto EXIT;
        }

        /* Look for code table null terminator in the offset field */
        while (be32_to_cpu(blkBuff[wrOffset]) != NULL)
        {
            /* Track the smallest code table offset */
            if ((be32_to_cpu(blkBuff[wrOffset]) < smallestOffset) || (smallestOffset == 0))
            {
                smallestOffset = be32_to_cpu(blkBuff[wrOffset]);
            }            
            
            /* Move on to next code table entry */
            wrOffset += ctLenWrds;
        }
    
        /* Verify space available for a new code table entry. wrOffset will be left at the 
         * null terminator (where the insert will happen).  smallestOffset will be the
         * entity closest to the code table. We will compare word counts below ...
         */
        smallestOffset = smallestOffset / sizeof(uint32_t); 
        if ( (smallestOffset <= wrOffset) || ((smallestOffset - wrOffset) <= (ctLenWrds + 1)) )
        {
            /* No room for a new entry */
            ASSERT(false);
            goto EXIT;
        }
    }

    /* Create a new code table entry in our buffer */
    blkBuff[wrOffset + ctOffOff]  = cpu_to_be32(nvStartOffset - ctOffset); /* Offset          */
    blkBuff[wrOffset + ctOffLen]  = cpu_to_be32(devData.BlockSize * 2);    /* Length          */
    blkBuff[wrOffset + ctOffId]   = cpu_to_be32(0xABCD0005);               /* ID/Type         */
    blkBuff[wrOffset + ctOffTerm] = 0;                        /* Null Terminator */

    /* Make sure the taget block is erased */
    if (OK != devErase(handle, (ctOffset/devData.BlockSize)))
    {
        goto EXIT;
    }

    /* Lay down the updated block */
    bytesWritten = devWrite(handle, (int8_t *)blkBuff, ctOffset, devData.BlockSize);
    if (bytesWritten == devData.BlockSize)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Container partition created on MEM_SPI0: REBOOTING!\n", 
                     __LINE__);
        retStatus = OK;            
        hwPlatformReset();
    }
    
#endif
EXIT:
    if (retStatus != OK)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_SHIM, "NV_DEV_SHIM(%d): Failed to create container partition on MEM_SPI0\n", 
                  __LINE__);
    }
    
    if (handle != NULL)
    {
        devClose(handle);
    }
    
    if (blkBuff != NULL)
    {
        MEM_FREE_AND_NULL(blkBuff);
    }
    
    return retStatus;
}


