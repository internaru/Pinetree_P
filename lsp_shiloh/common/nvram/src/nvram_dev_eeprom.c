/**
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_dev_eeprom.c
 *
 *  \brief NVRAM EEPROM device abstraction layer
 *
 *  This file manages blocks of NVRAM variables on EEPROM (I2C) devices: the 
 *  variable block storage algorithm for these devices does not need to implement
 *  wear leveling or erase before writing.  
 *
 *  This module manages the raw partition space provided by the partition manager API:
 *
 *  Partition start --->  +------------------------------------------+
 *                        | Block signature and fixed size mini      |
 *                        | code table (MAX_CODE_TABLE_LEN_EE bytes) |
 *                        +------------------------------------------+
 *                        | Variable blocks referenced by above      |
 *                        | table                                    |
 *                                            ... 
 *                        |                                          |
 *  Partition end --->    +------------------------------------------+
 *
 *  Note that this algorithm is very different than that used on SPI parts.
 *
 **/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "memAPI.h"
#include "dprintf.h"
#include "logger.h"
#include "nvram_logger.h"
#include "list.h"
#include "lassert.h"
#include "deviceid.h"
#include "nodeCache.h"
#include "nvram_api.h"
#include "nvram_dev_shim.h"
#include "nvram_dev_eeprom.h"
#include "partitionMgrAPI.h"


/*
 * Each entry in the EEPROM mini-code (variable block) table is three 32-bit words long:
 *    +----------+------------+----------+
 *    |  OFFSET  |   LENGTH   |   TYPE   |
 *    |   (0)    |    (1)     |   (2)    |
 *    +----------+------------+----------+
 *
 *  The first 32-bit word in the code table is actually the signature (EE_VAR_BLOCK_SIG),
 *  followed by the 3 word entries up to the maximum table size.  While the mini-code
 *  table entries look like the code table entries used by boot code, this is not 
 *  a requirement and the implementations are separate.
 */
#define EE_OFFSET_OFFSET       0        /**< Offset to the offset field (words)    */
#define EE_LEN_OFFSET          1        /**< Offset to the length field (words)    */
#define EE_TYPE_OFFSET         2        /**< Offset to the type field (words)      */
#define EE_TABLE_LEN           3        /**< Entry length in 32-bit words          */

#define MAX_CODE_TABLE_LEN_EE  64       /**< Byte table len: sig and 5 entries     */

#define MAX_EE_PARTSIZE        1500      /**< Total partition size limit, bytes     */

#define EE_VAR_BLOCK_SIG       0xBEEF   /**< EEPROM Container partition signature  */


/*
 * Local (static) Variables
 */
static ATLISTENTRY VarBlocksListI2C;   /* NV variable blocks on I2C (EEPROM) */
static bool        EepromInitComplete = false;


/*
 * Local (static) Functions
 */
static uint32_t getTableLenI2C(uint32_t *Add);
static uint32_t scanTableI2C(uint32_t *Add, char *label, uint32_t MaxTableLen);
static uint32_t GetUnusedEntryI2C(uint32_t *Add, uint32_t maxLen);


/*
 * FUNCTION NAME: InitNvramEEPROM 
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
void InitNvramEEPROM(void)
{
    if (EepromInitComplete == false)
    {
        EepromInitComplete = true;

        /* Initialize partition table list */
        memset(&VarBlocksListI2C, 0, sizeof(ATLISTENTRY));
        ATInitList(&VarBlocksListI2C);        
    }
}


/*
 * FUNCTION NAME: GetPartitionNeedsEEPROM 
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
error_type_t GetPartitionNeedsEEPROM(MEMORY_DEVICE memDev, uint32_t *partitionSize)
{
    /* REVISIT:
     *  This needs to be smarter (possibly look at config table and page sizes)
     */
    *partitionSize = MAX_EE_PARTSIZE;

    return OK;
}


/*
 * FUNCTION NAME: GetVarBlockSizeEEPROM
 *
 * See nvram_dev_eeprom.h for function documentation.
 */
uint32_t GetVarBlockSizeEEPROM(MEMORY_DEVICE memDev, char *label)
{
    BLOCK_NODE_t *myNode;
    uint32_t      retSize = 0;
    
    /* Look for the var block, set size if found */
    myNode = FindNodeByLabelAndDev(label, memDev, &VarBlocksListI2C);
    if (myNode != NULL)
    {
        retSize = myNode->Size;
    }
 
    return retSize;
}


/*
 * FUNCTION NAME: DiscoverVarBlocksEEPROM 
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
void DiscoverVarBlocksEEPROM(MEMORY_DEVICE memDev)
{
    uint32_t            offset;
    uint32_t            actualTableLen;
    PARTMGR_ERROR_t     partMgrErr;
    uint32_t            actualBytes;
    BLOCK_NODE_t       *newPartEntry;
    uint32_t           *buff   = NULL;
    partition_handle_t *handle = NULL;
    
    /* Verify that the passed values are in a sane state */
    ASSERT(MEM_I2C == memDev);

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Discover NV blocks on %s\n", __LINE__, getDevIdStr(memDev));

    /* Open our 'container' partition */
    partMgrErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &handle);
    if (partMgrErr != PARTMGR_OK)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): NV container partition not present\n", __LINE__);
        return;
    }

    /* NOTE:
     * Variable blocks are stored completely differently on I2C than on SPI!  All of 
     * the variable entries are stored in a fixed size mini-code table right after the 
     * partition signature.  Actual variable values are then stored between the end of 
     * the code table and the end of the partition.
     *
     * Allocate a RAM buffer for the mini-code table 
     */
    buff = (uint32_t *)MEM_MALLOC(MAX_CODE_TABLE_LEN_EE);
    if (buff == NULL)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Can't build dynamic table, alloc failed\n", __LINE__);
        goto EXIT;
    }

    /* Pull the mini-code table into RAM */    
    partMgrErr = ReadPartition(handle, (int8_t *)buff, 0, MAX_CODE_TABLE_LEN_EE, &actualBytes); 
    if ((partMgrErr != PARTMGR_OK) || (actualBytes != MAX_CODE_TABLE_LEN_EE))
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Can't build dynamic table, dev read failed\n", __LINE__);
        goto EXIT;
    }

    /*
     * If we have a brand new container partition the signature will
     * be missing.
     */
    if (buff[0] != EE_VAR_BLOCK_SIG)
    {
        /* New partition. Write the partition signature and zero out the entire
         * variable table to ensure proper termination of the list.
         */
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Initializing var block partition\n", __LINE__);
        memset(buff, 0, MAX_CODE_TABLE_LEN_EE);
        buff[0] = EE_VAR_BLOCK_SIG;
        
        partMgrErr = WritePartition(handle, (int8_t *)buff, 0, MAX_CODE_TABLE_LEN_EE, &actualBytes);
        if ((partMgrErr != PARTMGR_OK) || (actualBytes != MAX_CODE_TABLE_LEN_EE))
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Can't init var block partition, dev write failed\n", __LINE__);
            goto EXIT;
        }
    }

    /* Get the actual length of the mini-code table */
    actualTableLen = getTableLenI2C(buff);
    ASSERT(actualTableLen <= MAX_CODE_TABLE_LEN_EE);

    /* Now parse through the temporary RAM copy of the table; start at offset 1 because
     * the signature is at offset 0
     */
    offset = 1;
    while ((buff[offset] != 0) && (offset < actualTableLen))
    {
        newPartEntry = AllocNewNode();
        if (NULL != newPartEntry)
        {
            /* Invalid entry found, just skip it */
            if (buff[offset + EE_TYPE_OFFSET] == INVALID_ENTRY)
            {
                offset += EE_TABLE_LEN;
                continue;
            }

            newPartEntry->StartOffset = buff[offset + EE_OFFSET_OFFSET];
            newPartEntry->ID          = buff[offset + EE_TYPE_OFFSET];
            newPartEntry->Size        = buff[offset + EE_LEN_OFFSET];
            newPartEntry->MemDev      = memDev;

            /* Insert the partition in the dynamic partition list */
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): adding dynamic partition ID=0x%x\n", 
                      __LINE__, newPartEntry->ID);
            ATInsertTailList(&VarBlocksListI2C, &newPartEntry->listNode);
            offset += EE_TABLE_LEN;
        }
        else
        {
           /* REVISIT: 
            * Shouldn't we just break out of the loop? Are we really going to get
            * memory next time around?
            */
           DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Unable to allocate memory for dynamic partition table\n", __LINE__);
        }
    }

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): done searching for dynamic partitions on %s\n", __LINE__, getDevIdStr(memDev));

EXIT:
    /* Clean up and exit */
    if (handle != NULL)
    {
        ClosePartition(handle);
    }
    
    if (buff != NULL)
    {
        MEM_FREE_AND_NULL(buff);
    }
}


/*
 * FUNCTION NAME: AddRmVarBlockEEPROM 
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
error_type_t AddRmVarBlockEEPROM(char *label, MEMORY_DEVICE memDev, uint32_t maxSize, bool IsAdd)
{
    error_type_t        retStatus = FAIL;
    uint32_t            offset;
    uint32_t            tableLen = 0, dataOffset = 0;
    uint32_t            tmpDataLen = 0, tmpOffset = 0;
    uint32_t            actualBytes;
    PARTMGR_ERROR_t     partMgrErr;
    uint32_t           *tempBuff = NULL;
    BLOCK_NODE_t       *partEntry = NULL;
    partition_handle_t *handle = NULL;
    partition_info_t    partInfo;

    if (label == NULL)
    {
        return FAIL;
    }

    /* NOTE:
     * Variable blocks are stored completely differently on I2C than on SPI!  All of 
     * the variable entries are stored in a fixed size mini-code table right after the 
     * partition signature.  Actual variable values are then stored between the end of 
     * the code table and the end of the partition.
     *
     * Allocate a RAM buffer for the mini-code table 
     */
    tempBuff = (uint32_t *)MEM_MALLOC(MAX_CODE_TABLE_LEN_EE);
    if (tempBuff == NULL)
    {
        goto EXIT;
    }

    /* Open our 'container' partition */
    partMgrErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, memDev, &handle);
    if (partMgrErr != PARTMGR_OK)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): NV var partition not present on %s\n", __LINE__, getDevIdStr(memDev));
        goto EXIT;
    }

    /* Pull the mini-code table into RAM */    
    partMgrErr = ReadPartition(handle, (int8_t *)tempBuff, 0, MAX_CODE_TABLE_LEN_EE, &actualBytes); 
    if ((partMgrErr != PARTMGR_OK) || (actualBytes != MAX_CODE_TABLE_LEN_EE))
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Can't build table, dev read failed\n", __LINE__);
        goto EXIT;
    }

    /* Get the actual table length */
    tableLen = getTableLenI2C(tempBuff);
 
    /* Check for code table length more than max table length size */
    if (tableLen > MAX_CODE_TABLE_LEN_EE)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): table length is more than maximum\n", __LINE__);
        goto EXIT;
    }

    /* Get to offset for requested variable block entry in mini-code table */
    offset = scanTableI2C(tempBuff, label, MAX_CODE_TABLE_LEN_EE);

    if (IsAdd == REMOVE)
    {
        /* REMOVE case: can't do the remove if block not present */
        if ((offset == 0) || (offset >= MAX_CODE_TABLE_LEN_EE))
        {
            /* Entry not found, bail out */
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): unable to remove '%4s'\n", 
                      __LINE__, label);
            goto EXIT;
        }

        /* Mark the entry type as invalid (do not alter offset and length) */
        tempBuff[offset + EE_TYPE_OFFSET] = INVALID_ENTRY;

        /* Check if it is the last table entry, if so terminate list */
        if (tableLen == (offset + EE_TABLE_LEN))
        {
            tempBuff[offset] = 0;
        }
    }
    else
    {
        /* ADD case: make sure block doesn't already exist */
        if (offset != MAX_CODE_TABLE_LEN_EE)
        {
            /* Already exists, but only return OK if sizes match */
            if (tempBuff[offset + EE_LEN_OFFSET] == maxSize)
            {
                retStatus = OK;
            }
            goto EXIT;
        }

        if (tableLen > 1)
        {
            /* Code table entries exist: try to find open entry */
            offset = GetUnusedEntryI2C(tempBuff, maxSize);
            if (offset == MAX_CODE_TABLE_LEN_EE)
            {
                /* No open entries, so append at the end of the table */
                offset = tableLen;
                tableLen += EE_TABLE_LEN;
            }
        }
        else
        {
            /* No code table entries, put in first position */
            offset = 1;
        }

        /* Calculate new data offset */
        if (tableLen <= 1)
        {
            dataOffset = MAX_CODE_TABLE_LEN_EE;
        }
        else
        {
            tmpDataLen = tempBuff[(offset - EE_TABLE_LEN) + EE_LEN_OFFSET];
            tmpOffset  = tempBuff[(offset - EE_TABLE_LEN) + EE_OFFSET_OFFSET];
            dataOffset = tmpDataLen + tmpOffset;
            //dataOffset = tempBuff[(offset - DY_TABLE_LEN) + DY_LEN_OFFSET] +
            //         tempBuff[(offset - DY_TABLE_LEN) + DY_OFFSET_OFFSET];
        }

        /* If not enough space available */
        if (GetPartitionInfoByHandle(handle, &partInfo) != PARTMGR_OK)
        {
            ASSERT(false);
        }
        
        if ( ((tableLen + EE_TABLE_LEN) > MAX_CODE_TABLE_LEN_EE) || 
             ((dataOffset + maxSize) > partInfo.Size) )
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d):Insufficient space in varible section on %s for '%4s'\n", 
                      __LINE__, getDevIdStr(memDev), label);
            goto EXIT;
        }

        /* Update the table */
        memcpy((char *)&tempBuff[offset + EE_TYPE_OFFSET], label, 4);
        tempBuff[offset + EE_LEN_OFFSET]    = maxSize;
        tempBuff[offset + EE_OFFSET_OFFSET] = dataOffset;
        
        /* Terminate list if this is now the last entry */
        if ((offset + EE_TABLE_LEN) == tableLen)
        {
            tempBuff[offset + EE_TABLE_LEN] = 0;
        }
    }

    /* Try to write mini-code table back to EE part */
    partMgrErr = WritePartition(handle, (int8_t *)tempBuff, 0, MAX_CODE_TABLE_LEN_EE, &actualBytes);
    if ((partMgrErr == PARTMGR_OK) && (actualBytes == MAX_CODE_TABLE_LEN_EE))
    {
        /* Physical part is now updated, so update our RAM based table */
        if (IsAdd == true)
        {
            /* Allocate space to ADD new entry */
            partEntry = AllocNewNode();
            if (partEntry == NULL)
            {
                /* REVISIT:
                 * The physical part is updated but our RAM table is NOT: this means
                 * no access to the block until a reboot.  This seems like badness:
                 * perhaps the RAM sould get allocated before the Write() above.
                 */
                goto EXIT;
            }

            /* Create variable block entry */
            memcpy(&partEntry->Label[0], label, 4);
            partEntry->Size        = maxSize;
            partEntry->StartOffset = dataOffset;
            partEntry->MemDev      = memDev;

            /* Insert in partition list */
            ATInsertTailList(&VarBlocksListI2C, &partEntry->listNode);
        }
        else
        {
            /* REMOVE entry: find it and delete it */
            partEntry = FindNodeByLabelAndDev(label, memDev, &VarBlocksListI2C);
            if (partEntry)
            {
                ATRemoveEntryList(&partEntry->listNode);
                MEM_FREE_AND_NULL(partEntry);
            }
        }
        
        /* Add/Remove passed if we made it here */
        retStatus = OK;
    }
    else
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM,"NV_DEV_EE(%d): Unable to update nvram table\n", __LINE__);
    }

 EXIT:
    /* Clean up and exit */
    if (handle != NULL)
    {
        ClosePartition(handle);
        handle = NULL;
    }
    
    if (tempBuff != NULL)
    {
        MEM_FREE_AND_NULL(tempBuff);
    }

    return retStatus;
}


/* FUNCTION NAME: VarBlockReadEEPROM
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
uint32_t VarBlockReadEEPROM(nvram_handle_t   *nHandle, 
                            int8_t         *dest,
                            uint64_t        offset, 
                            uint32_t        size)
{
    PARTMGR_ERROR_t     partMgrErr;
    BLOCK_NODE_t       *partEntry = NULL;
    partition_handle_t *pHandle = NULL;
    uint32_t            readCnt = 0;
        
    ASSERT(nHandle != NULL);
    ASSERT(dest != NULL);
    ASSERT(size != 0 );
 
    /* Attempt to find the variable block */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, &VarBlocksListI2C);
    if (partEntry == NULL)
    {
        return 0;
    }

    /* Make sure we aren't reading off end of our var block */
    if ((offset + size) > partEntry->Size)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): attempt to read past end of var block\n", __LINE__);
        return 0;
    }
     
    /* Open our 'container' partition */
    partMgrErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, nHandle->DeviceID, &pHandle);
    if (partMgrErr != PARTMGR_OK)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): NV container partition not present on %s\n", 
                      __LINE__, getDevIdStr(nHandle->DeviceID));
        return 0;
    }

    /* Read the device */
    partMgrErr = ReadPartition(pHandle, dest, (partEntry->StartOffset + offset), size, &readCnt);
    if ((partMgrErr != PARTMGR_OK) || (readCnt != size))
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): device read failure\n", __LINE__);
        ClosePartition(pHandle);
        return 0;
    }

    ClosePartition(pHandle);
    return readCnt;  
}


/* FUNCTION NAME: VarBlockWriteEEPROM
 *
 *  See nvram_dev_eeprom.h for function documentation.
 */
uint32_t VarBlockWriteEEPROM(nvram_handle_t   *nHandle, 
                             int8_t         *src)
{
    PARTMGR_ERROR_t     partMgrErr;
    BLOCK_NODE_t       *partEntry = NULL;
    partition_handle_t *pHandle = NULL;
    uint32_t            writeCnt;
        
    ASSERT(nHandle != NULL);
    ASSERT(src != NULL);
    ASSERT(nHandle->MaxLength != 0 );
 
    /* Attempt to find the variable block */
    partEntry = FindNodeByLabelAndDev(nHandle->BlockLabel, nHandle->DeviceID, &VarBlocksListI2C);
    if (partEntry == NULL)
    {
        return 0;
    }

    /* Make sure we aren't writing a bad size block */
    if (nHandle->MaxLength != partEntry->Size)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): block size mismatch\n", __LINE__);
        return 0;
    }
     
    /* Open our 'container' partition */
    partMgrErr = OpenPartition(VARBLOCK_PART_ID, IM_NV_STORE, nHandle->DeviceID, &pHandle);
    if (partMgrErr != PARTMGR_OK)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): NV container partition not present on %s\n", 
                      __LINE__, getDevIdStr(nHandle->DeviceID));
        return 0;
    }

    /* Write the device */
    partMgrErr = WritePartition(pHandle, src, partEntry->StartOffset, nHandle->MaxLength, &writeCnt);
    if ((partMgrErr != PARTMGR_OK) || (nHandle->MaxLength != writeCnt))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): device write failure\n", __LINE__);
        ClosePartition(pHandle);
        return 0;
    }

    ClosePartition(pHandle);
    return writeCnt;      
}



/**
 *  \brief Finds the length of dynamic partition table on an I2C device
 *
 *  \param[in] *Add     Address to the I2C partition
 *
 *  \return offset
 *
 **/
static uint32_t getTableLenI2C(uint32_t *Add)
{
    uint32_t offset = 1;

    DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM, "NV_DEV_EE(%d): Var table info 0=0x%x 1=0x%x 2=0x%x 3=0x%x\n", 
              __LINE__, Add[0], Add[1], Add[2], Add[3]);

    /* Verify the variable block signature */
    if (Add[0] != EE_VAR_BLOCK_SIG)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM_DEV_EEPROM,"NV_DEV_EE(%d): invalid partition key\n", __LINE__);
        return 0; 
    }
    
    /* Run through table until we hit terminator or max length */
    while (Add[offset] != 0)
    {
        offset += EE_TABLE_LEN;
        if (offset >= MAX_CODE_TABLE_LEN_EE)
        {
            return MAX_CODE_TABLE_LEN_EE;
        }
    }
    
    return offset;
}


/**
 *  \brief Scans the code table on I2C device to find the given partition ID
 *
 *  \param[in] *Add           Address to the code table
 *  \param[in] *label         Partition label to be searched
 *  \param[in] MaxTableLen    Length of code table
 *
 *  \return error_type_t
 *  \retval OK 0
 *  \retval FAIL -1
 **/
static uint32_t scanTableI2C(uint32_t *Add, char *label, uint32_t MaxTableLen)
{
    uint32_t offset = 1;

    while (Add[offset] != 0)
    {
        if (memcmp((char *)&Add[offset + EE_TYPE_OFFSET], label, 4) == 0)
        {
            /* we found the partition, return offset */
            return offset;
        }
        
        /* go to the next table entry */
        offset += EE_TABLE_LEN;
        if (offset >= MaxTableLen)
        {
            return MaxTableLen;
        }
    }
    
    return MaxTableLen;
}

/**
 *
 *  \brief Search for unused entry and offset in code table
 *
 *  \param[in] *Add Address to start the scan
 *
 *  \return uint32_t
 *  \retval offset to unused entry in the table
 *
 **/
static uint32_t GetUnusedEntryI2C(uint32_t *Add, uint32_t maxLen)
{
    uint32_t offset = 1;

    /* Validate the mini-code table is properly signed */
    if (Add[0] != EE_VAR_BLOCK_SIG)
    {
        ASSERT(false);
    }

    while(Add[offset] != 0)
    {
        /* Invalid slot in code table, maybe we can use it */
        if (Add[offset + EE_TYPE_OFFSET] == INVALID_ENTRY)
        {
            if (Add[offset + EE_LEN_OFFSET] >= maxLen)
            {
                return offset;
            }
        }
        
        /* Move on to the next entry, be careful not to run off end of table */
        offset += EE_TABLE_LEN;
        if (offset >= MAX_CODE_TABLE_LEN_EE)
        {
            return MAX_CODE_TABLE_LEN_EE;
        }
    }
    
    return MAX_CODE_TABLE_LEN_EE;
}
