/*
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvm_config.c
 *
 *  \brief Non-Volatile Memory Manager Configuration
 *
 *  Please see nvm_config.h for interface/usage documentation.
 *
 **/

#include "deviceid.h"
#include "nvram_config.h"



/*
 * The ROM Manager search path, ordered from highest to lowest priority.  List must
 * be terminated with "MEM_DEVICE_NOT_FOUND'.  Entities must be found on the 
 * search path to be used.
 */
#define ROMMGR_SEARCH_INIT  { MEM_I2C, MEM_SPI0, MEM_DEVICE_NOT_FOUND }


/*
 * The NVM Manager store search path, ordered from highest to lowest priority.  List 
 * must be terminated with "MEM_DEVICE_NOT_FOUND'.
 */
#define NVSTORE_SEARCH_INIT  { MEM_SPI0, MEM_NAND0, MEM_I2C, MEM_DEVICE_NOT_FOUND }


/*
 *  Local (static) variables
 */
static MEMORY_DEVICE RommgrSearchList[]  = ROMMGR_SEARCH_INIT;
static MEMORY_DEVICE NvStoreSearchList[] = NVSTORE_SEARCH_INIT;


/*
 * FUNCTION NAME: getRommgrSearchList 
 *
 * Please see nvram_config.h for function documentation.
 */
const MEMORY_DEVICE *getRommgrSearchList(void)
{    
    return RommgrSearchList;
}


/*
 * FUNCTION NAME: getNvStoreSearchList 
 *
 * Please see nvram_config.h for function documentation.
 */
const MEMORY_DEVICE *getNvStoreSearchList(void)
{
    return NvStoreSearchList;
}


#if defined(HAVE_NAND)
#include "platform.h"
#include "memAPI.h"
#include "code_table.h"
#include "nvram_dev_shim.h"
#include "imageMapUtil.h"
#include "memdevice.h"
#include "dprintf.h"
#include "logger.h"
#include <string.h>

error_type_t WriteFullCodetable(MEMDEV_HANDLE   *devHandle, 
                                uint8_t         *data, 
                                uint64_t        offset, 
                                DRIVER_CONFIG_t *devData)
{
    uint32_t nvAddress;
    uint32_t nvLength;

    DPRINTF(LOG_ERR,("%s()\r\n",__FUNCTION__));
    // failed so erase and build from scratch
    memset(data, 0xff, devData->PageSize);
    ct_build_header((uint32_t *)data, 1);   // build a header

    // locate a version/cookie marker of OUR default map
    ct_add_entry(data, IM_CODETABLE_LAYOUT, CODETABLE_VERSION, 0, 0);  

    // location filesystem
    nvAddress = NAND_MAP_FILE_SYS_START * devData->BlockSize;
    nvLength  = NUM_BLKS_FILE_SYS * devData->BlockSize;
    ct_add_entry(data, IM_FILE_SYS_STORE, 0, nvAddress, nvLength);

    // instead of putting table address, just store SPARE pool address/size
    nvAddress = NAND_MAP_SPARE_POOL_START * devData->BlockSize;
    nvLength  = NUM_BLKS_SPARING * devData->BlockSize;
    ct_add_entry(data, IM_SPARE_SECTOR, 0, nvAddress, nvLength);
    ct_complete_table(data);

    // Make sure the target block is erased, AS WELL as rest of image. 
    // This will erase even our RBBT, but we will create next time through.
    // If anything needed sparing on first boot, it will write to correct location due
    // to our DRAM table..
    uint32_t block = (offset/devData->BlockSize);
    DPRINTF(LOG_ERR,("%s() WARNING - erasing image\r\n",__FUNCTION__));
    do
    {
        if (OK != devErase(devHandle, block))
        {
            return FAIL;
        }
    } while (++block < devData->BlockCount);

    /* Lay down the updated block */
    if (devData->PageSize == devWrite(devHandle, (int8_t *)data, offset, devData->PageSize))
    {
        return OK;
    }
    return FAIL;
}

error_type_t nvconfig_nand_format(DEVICE_HANDLE memDev)
{
    DRIVER_CONFIG_t devData;
    uint8_t *data;
    error_type_t retval = FAIL;

    DPRINTF(LOG_ERR,("%s\n",__FUNCTION__));

    /* Get block size and block count information */
    if (OK != devReport(memDev, &devData))
    {
        return retval;
    }

    data = (uint8_t *)MEM_MALLOC(devData.PageSize);

    retval = WriteFullCodetable(memDev, data, (NAND_MAP_CODETABLE_BLOCK_START * devData.BlockSize), &devData);

    MEM_FREE_AND_NULL(data);
    return retval;
}

/**
 * nvconfig_nand_verify_format - for this platform, verifies 
 * that the passed in code table buffer meets minimum format 
 * requirements... 
 * 
 * @param ctbuf - pointer to data buffer containing a known 
 *              valid config table where known valid indicates
 *              that the table has already passed length and
 *              checksum verifcation.
 * @return error_type_t 
 */
error_type_t nvconfig_nand_verify_format(uint8_t* ctbuf)
{
    uint32_t entry;
    uint32_t entry_id;

    // if our codetable layout version is good, we are fine
    entry = ct_find_entry((uint32_t *)ctbuf, IM_CODETABLE_LAYOUT, 1);
    if (!entry)
    {
        DPRINTF(LOG_ERR,("%s() IM_CODETABLE_LAYOUT not found\r\n",__FUNCTION__));
        return FAIL;
    }

    entry_id = ct_get_entry_id((uint32_t *)ctbuf, entry);
    if (entry_id < CODETABLE_VERSION)
    {
        DPRINTF(LOG_ERR,("%s() IM_CODETABLE_LAYOUT too old: %#x < %#x \r\n",__FUNCTION__,entry_id,CODETABLE_VERSION));
        return FAIL;
    }
    //DPRINTF(LOG_ERR,("%s() IM_CODETABLE_LAYOUT OK: %#x >= %#x \r\n",__FUNCTION__,entry_id,CODETABLE_VERSION));

    return OK;
}

#endif

