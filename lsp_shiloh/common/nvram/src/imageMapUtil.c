/*
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file imageMapUtil.c
 *
 * \brief Image map utilities
 *
 * This file implements the image map utility functions, please see the 
 * imageMapUtil.h file for detailed interface descriptions.
 *
 **/
  
#include <stdint.h>
#include <string.h>
#include "error_types.h"
#include "list.h"
#include "memAPI.h"
//#include "debug.h"
#include "lassert.h"
#include "platform.h"
#include "nvram_config.h"
#include "deviceid.h"
#include "memdevice.h"
#include "nodeCache.h"
#include "imageMapUtil.h"
#include "nvram_logger.h"
#include "ioutils.h"
#ifdef IMAGEMAP_VER_1_0
#include "code_table.h"
#endif
#include <pthread.h>
#include "posix_ostools.h"

#ifdef IMAGEMAP_VER_1_0
static ALIGN(32) code_table_header_t imHdr;
static error_type_t FindImageMap_1_0(MEMORY_DEVICE memDev, ImageMapInfo_t *imInfo)
{
    MEMDEV_HANDLE    *devHandle;
    DRIVER_CONFIG_t   devData;
    int8_t           *ctBuff;
    uint64_t          ctAddr;
    uint32_t          length;
    uint32_t          i;
    int32_t           retry_count = 1;

    struct timespec start_time, end_time, elapsed_time;

    /* Parameter sanity check */
    if (imInfo == NULL)
    {
        return FAIL;
    }

    /* Init the return values just in case we bail out early */
    memset((int8_t *)imInfo, 0, sizeof(ImageMapInfo_t));    
    imInfo->MemDev        = memDev;
    imInfo->validImageMap = false;
    imInfo->imHdrLen      = sizeof(code_table_header_t);
    imInfo->imEntryLen    = sizeof(code_table_entry_t);
    imInfo->imTerm        = 0;
    
    uint64_t Address = 0;  // default address  

    clock_gettime( CLOCK_REALTIME, &start_time );

    /* Open the memory device so we can read the code table */
    if (OK == devOpen(memDev, &devHandle))
    {
        /* Get block size and block count information */
        if (OK != devReport(devHandle, &devData))
        {
            devClose(devHandle);
            return FAIL;
        }        

        DBG_PRINTF(LOG_DEBUG|DBG_IMAGEMAP, "IM_UTIL(%s): Searching part %s\n", __func__, getDevIdStr(memDev));

        do
        {
            // for each block in the part
            for (i = 0; i < devData.BlockCount; i++)
            {
                Address = i * devData.BlockSize;

                //DBG_PRINTF(LOG_DEBUG|DBG_IMAGEMAP, "IM_UTIL(%s): Searching part %s block %d, addr %#x\n", __func__, getDevIdStr(memDev), i, (uint32_t)Address);

                /* Attempt to read the code table header at address 0(START_CODETABLE) */
                if (devRead(devHandle, (int8_t *) &imHdr, Address, imInfo->imHdrLen) != imInfo->imHdrLen)
                {
                    DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP, "IM_UTIL(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
                    devClose(devHandle);
                    return FAIL;
                }

                if ((length = ct_get_total_length(&imHdr)) != 0)
                {
                    // we have a code table.  See if we have all the code table loaded.
                    //
                    uint64_t    compilerBug = Address;
                    imInfo->imOffset      = compilerBug;
                    imInfo->validImageMap = true;

                    // read in the entire table and verify it
                    // when we allocate make sure that we have full cache lines, thus the
                    // addition of the cache line size to the length.
                    //
                    ctBuff = MEM_MALLOC_ALIGN(length+cpu_get_dcache_line_size(), cpu_get_dcache_line_size());
                    ASSERT(ctBuff);
                    ctAddr = imInfo->imOffset;
                    if (devRead(devHandle, (int8_t *)ctBuff, ctAddr, length) != length)
                    {
                        DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP, "IM_UTIL(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
                        devClose(devHandle);
                        memFree(ctBuff);
                        return FAIL;
                    }
                    // verify the code table is valid by checking the crc.
                    if (ct_verify_table((code_table_header_t *)ctBuff))
                    {
                        imInfo->imLenBytes = length;    // total length of the table
                        imInfo->imNumEntries = ct_get_num_entries(&imHdr);   // number of entries.

                        // table checksum passes.  now verfy format for those platforms that support
                        // format verification
                        if(devVerifyNvFormat(devHandle, (uint8_t*)ctBuff) != OK)
                        {
                            imInfo->validImageMap = false;
                        }
                    }
                    else
                    {
                        imInfo->validImageMap = false;
                    }

                    memFree(ctBuff);    // return the buffer.
                }
                if (imInfo->validImageMap)
                {
                    break;
                }
            }

            DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP, "IM_UTIL(%d): validImageMap = %d, retry_count = %d\n", __LINE__, imInfo->validImageMap, retry_count);

            if (!imInfo->validImageMap && retry_count > 0)
            {
                // If we could't find a valid code table try to format and then read the code table
                if(devFormat(devHandle) != OK)
                {
                    // format failed so don't try to re-read the code table
                    retry_count = 0;
                }
            }

        }while(!imInfo->validImageMap && retry_count-- > 0);

        /* Close our memory device */
        devClose(devHandle);
    }

    clock_gettime( CLOCK_REALTIME, &end_time );
    posix_timespec_sub( &end_time, &start_time, &elapsed_time );
    
    if (imInfo->validImageMap == false)
    {
        DBG_PRINTF(LOG_INFO|DBG_IMAGEMAP, "IM_UTIL(%d): Code table not found on %s. Time sec=%ld nsec=%ld\n", __LINE__, getDevIdStr(memDev), elapsed_time.tv_sec, elapsed_time.tv_nsec);
        return FAIL;
    }
    
    DBG_PRINTF(LOG_DEBUG|DBG_IMAGEMAP, "IM_UTIL(%d): Code table found on %s Time sec=%ld nsec=%ld\n", __LINE__, getDevIdStr(memDev), elapsed_time.tv_sec, elapsed_time.tv_nsec);
        
    return OK;
}
#endif // IMAGEMAP_VER_1_0

#ifdef IMAGEMAP_VER_0_0
static error_type_t FindImageMap_0_0(MEMORY_DEVICE memDev, ImageMapInfo_t *imInfo)
{
    MEMDEV_HANDLE    *devHandle;
    DRIVER_CONFIG_t   devData;
    int8_t           *ctBuff;
    uint64_t          ctAddr;
    code_table_header_t imHdr;

       
    /* Parameter sanity check */
    if (imInfo == NULL)
    {
        return FAIL;
    }

    /* NOTE:
     * Newer image map versions have a header that give all sorts of great data:
     * we don't have one so will derive or hard-code constants here.
     */

    /* Init the return values just in case we bail out early */
    memset((int8_t *)imInfo, 0, sizeof(ImageMapInfo_t));    
    imInfo->MemDev        = memDev;
    imInfo->validImageMap = false;
    imInfo->imHdrLen      = sizeof(code_table_header_t);
    imInfo->imEntryLen    = sizeof(code_table_entry_t);
    imInfo->imTerm        = NULL;
    
    uint64_t Address = 0;  // default address  

    /* Open the memory device so we can read the code table */
    if (OK == devOpen(memDev, &devHandle))
    {
        /* Get block size and block count information */
        if (OK != devReport(devHandle, &devData))
        {
            devClose(devHandle);
            return FAIL;
        }        
        /* Attempt to read the code table header at address 0(START_CODETABLE) */
        if (devRead(devHandle, (int8_t *) &imHdr, Address, imInfo->imHdrLen) != imInfo->imHdrLen)
        {
            DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP, "IM_UTIL(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
            devClose(devHandle);
            return FAIL;
        }

        code_table_entry_t  *imEntry;
        /* Check for the code table signature */
        if ( be32_to_cpu(imHdr.Signature) == CODE_KEY )
        {
            /* Code table found at address 0 */
            DBG_PRINTF(LOG_INFO|DBG_IMAGEMAP, "IM_UTIL(%d): Code table at block 0 on %s\n", __LINE__, getDevIdStr(memDev));
            imInfo->imOffset      = 0;
            imInfo->validImageMap = true;
        }
        else
        {
            /* We didn't find the code key where expected (first word on block 0).
             * SPI parts may contain boot code in the first block, forcing the code
             * table to block 1.  Does the SPI part really have boot code in block
             * 0 ... see if it's erased.
             */
            if ((memDev == MEM_SPI0) && (*((uint32_t *)&imHdr) != 0xffffffff))
            {
                /* Block 0 has data, assume it's bootcode.  Read at the next block boundary */
                if (devRead(devHandle, (int8_t *)&imHdr, devData.BlockSize, imInfo->imHdrLen) != imInfo->imHdrLen)
                {
                    DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP, "IM_UTIL(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
                    devClose(devHandle);
                    return FAIL;
                }

                if ( be32_to_cpu(imHdr.Signature) == CODE_KEY )
                {
                    /* Code table found in block 1 */
                    DBG_PRINTF(LOG_INFO|DBG_IMAGEMAP, "IM_UTIL(%d): Code table at block 1 on %s\n", __LINE__, getDevIdStr(memDev));
                    imInfo->imOffset      = devData.BlockSize;
                    imInfo->validImageMap = true;
                }
            }
        }

        /* If we have a valid code table, scan it for its length */        
        if (imInfo->validImageMap == true)
        {
            ctBuff = MEM_MALLOC(MAX_CODE_TABLE_LEN);
            if (ctBuff == NULL)
            {
                devClose(devHandle);
                return FAIL;
            }
 
            /* Pull the code table into our ram buffer */
            ctAddr = imInfo->imOffset + imInfo->imHdrLen;
            if (devRead(devHandle, ctBuff, ctAddr, MAX_CODE_TABLE_LEN) != MAX_CODE_TABLE_LEN)
            {
                DBG_PRINTF(LOG_ERR|DBG_IMAGEMAP,"IM_UTIL(%d): ERROR can't read %s\n", __LINE__, getDevIdStr(memDev));
                devClose(devHandle);
                memFree(ctBuff);
                return FAIL;
            }

            /* Count the number of code table entries */
            imEntry = (code_table_entry_t *)ctBuff;
            while ( be32_to_cpu(imEntry->Offset) != imInfo->imTerm)
            {
                imInfo->imNumEntries++;
                imEntry++;
            }
            
            /* Set overall code table length */
            imInfo->imLenBytes = imInfo->imHdrLen + (imInfo->imNumEntries * imInfo->imEntryLen);
            
            memFree(ctBuff);
        }

        /* Close our memory device */
        devClose(devHandle);
    }

    if (imInfo->validImageMap == false)
    {
        DBG_PRINTF(LOG_INFO|DBG_IMAGEMAP, "IM_UTIL(%d): Code table not found on %s\n", __LINE__, getDevIdStr(memDev));
        return FAIL;
    }
    
    return OK;
}
#endif // IMAGEMAP_VER_0_0

#ifdef IMAGEMAP_VER_1_0
static error_type_t ReadImageMapEntry_1_0(ImageMapInfo_t  *imInfo,
                               DRIVER_CONFIG_t *devInfo,
                               int8_t          *imBuffer,
                               uint32_t         entryNum,
                               BLOCK_NODE_t    *partEntry)
{
    uint32_t          partBlkCnt;
   
    /* Parameter sanity check */
    if ((imInfo == NULL) || (imBuffer == NULL) || (partEntry == NULL) || (devInfo == NULL))
    {
        ASSERT(false);
        return FAIL;
    }

    /* Make sure the entry is in range (starts at 0) */
    if (entryNum >= imInfo->imNumEntries)
    {
        return FAIL;
    }

    entryNum++;     // must be > 0, this will be zero with the loop.
    partEntry->MemDev = imInfo->MemDev;
    partEntry->ID = ct_get_entry_id((uint32_t *)imBuffer, entryNum);
    partEntry->Type = ct_get_entry_type((uint32_t *)imBuffer, entryNum);
    partEntry->Size = ct_get_entry_length((uint32_t *)imBuffer, entryNum);
    partEntry->StartOffset = ct_get_entry_offset((uint32_t *)imBuffer, entryNum);
    partEntry->StartBlock  = partEntry->StartOffset / devInfo->BlockSize;

    /* Calculate the partition size (rounded up to blocks) */
    partBlkCnt = partEntry->Size / devInfo->BlockSize;
    if (partEntry->Size % devInfo->BlockSize)
    {
         partBlkCnt += 1;
    }
    partEntry->BlockCount = partBlkCnt;
 
    return OK;
}
#endif // IMAGEMAP_VER_1_0

#ifdef IMAGEMAP_VER_0_0
static error_type_t ReadImageMapEntry_0_0(ImageMapInfo_t  *imInfo,
                               DRIVER_CONFIG_t *devInfo,
                               int8_t          *imBuffer,
                               uint32_t         entryNum,
                               BLOCK_NODE_t    *partEntry)
{
    uint32_t          partBlkCnt;
   
    /* Parameter sanity check */
    if ((imInfo == NULL) || (imBuffer == NULL) || (partEntry == NULL) || (devInfo == NULL))
    {
        ASSERT(false);
        return FAIL;
    }

    /* Make sure the entry is in range (starts at 0) */
    if (entryNum >= imInfo->imNumEntries)
    {
        return FAIL;
    }

    code_table_entry_t  *imEntry;
    /* Set our buffer pointer */
    imEntry = (code_table_entry_t *)(imBuffer + imInfo->imHdrLen);

    /* Fill in the object info */
    partEntry->MemDev      = imInfo->MemDev;
    partEntry->ID          = be16_to_cpu(imEntry[entryNum].ID);
    partEntry->Type        = be16_to_cpu(imEntry[entryNum].Type);
    partEntry->Size        = be32_to_cpu(imEntry[entryNum].Length);
    partEntry->StartOffset = be32_to_cpu(imEntry[entryNum].Offset);
    
    partEntry->StartBlock  = partEntry->StartOffset / devInfo->BlockSize;

    /* Calculate the partition size (rounded up to blocks) */
    partBlkCnt = partEntry->Size / devInfo->BlockSize;
    if (partEntry->Size % devInfo->BlockSize)
    {
         partBlkCnt += 1;
    }
    partEntry->BlockCount = partBlkCnt;
 
    return OK;
}
#endif // IMAGEMAP_VER_0_0


/* FUNCTION NAME: FindImageMap
 *
 * Please see imageMapUtil.h for function documentation.
 */
error_type_t FindImageMap(MEMORY_DEVICE memDev, ImageMapInfo_t *imInfo)
{
#if defined(IMAGEMAP_VER_1_0)

    return FindImageMap_1_0(memDev, imInfo);

#elif defined(IMAGEMAP_VER_0_0)

    return FindImageMap_0_0(memDev, imInfo);

#else
#error "Image map version not defined."
#endif
}

/* FUNCTION NAME: ReadImageMapEntry
 *
 * Please see imageMapUtil.h for function documentation.
 */
error_type_t ReadImageMapEntry(ImageMapInfo_t  *imInfo,
                               DRIVER_CONFIG_t *devInfo,
                               int8_t          *imBuffer,
                               uint32_t         entryNum,
                               BLOCK_NODE_t    *partEntry)
{
#if defined(IMAGEMAP_VER_1_0)

    return ReadImageMapEntry_1_0(imInfo, devInfo, imBuffer, entryNum, partEntry);

#elif defined(IMAGEMAP_VER_0_0)

    return ReadImageMapEntry_0_0(imInfo, devInfo, imBuffer, entryNum, partEntry);

#else
#error "Image map version not defined."
    return FAIL;
#endif
}

