/*
 *
 * ============================================================================
 * Copyright (c) 2006-2009   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file writeeeprom.c
 *
 * \brief Write a file to the eeprom.
 *
 */

#include <stdint.h>
#include <string.h>

#include "ATypes.h"
#include "memAPI.h"
#include "logger.h"
#include "acl_api.h"
#include "aclproto.h"
#include "lassert.h"
#include "acl_nvram_constants.h"
#include "checksum.h"

#include "deviceid.h"
#include "partitionMgrAPI.h"
#include "ioutils.h"

#define ACLCMD_WRITE_EEPROM        0xEC0D


#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    uint32_t dataLength;
    uint8_t temp[8];
}writeEEPROMCmd_t;
 
#define CONFIG_SECTOR     1
#pragma pack()
/* FUNCTION NAME: AclWriteEEPROM */
 
/** 
 * \brief Write the next data to the eeprom
 * 
 * \param[in] Pipe  IO Pipe for reporting results to the host
 * 
 * \param[in] AclCmd The command block that caused this to be called.
 * 
 * \param reserved Save for later
 * 
 * \retval int32_t 0 success, 1 failure.
 * 
 * \date 2/27/2006
 * 
 **/
error_type_t AclWriteEEPROM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    int8_t *DataBuf;
    uint32_t size;
    ATSTATUS status=AT_SUCCESS;
    int32_t bytesRead;
    acl_response_base_struct_t *Buffer;
    uint32_t length;
    uint32_t   crcVal, testcrc;

    const MEMORY_DEVICE *searchList;
    MEMORY_DEVICE        targetDev;
    partition_handle_t  *Handle;
    partition_info_t     Info;

    // Initialize the response buffer.
    //
    Buffer = construct_acl_response_buffer(ACLCMD_WRITE_EEPROM);
    //
    // Get the size of the data coming down.
    //
    size = be32_to_cpu( ((writeEEPROMCmd_t *)AclCmd)->dataLength );

    DataBuf = (int8_t *)MEM_MALLOC( size );
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Download is %d bytes\n", size));

    if ( DataBuf != NULL )
    {
        //
        // We have the data buffer allocated, not read the data into the buffer.
        //
        bytesRead = Pipe->ReadConnect(Pipe, (void *)DataBuf, size, PARSEREAD_EXACT );
        if (bytesRead < 0)
        {
            status = AT_FAILURE;      // force an error if the read failed.
        }

    }
    else
    {
        status = AT_FAILURE;
    }
    // if we did not read anything, skip checking stuff.
    if (status != AT_FAILURE)
    {
        //
        // Test the crc
        // This checks the tranmission crc.
        //
        size -= CHECKSUM_LENGTH;
        memcpy(&testcrc,&DataBuf[size],CHECKSUM_LENGTH);
		testcrc = be32_to_cpu(testcrc);
        crcVal = CalcChecksum((uint32_t *)DataBuf, size);
        if (crcVal != testcrc)
        {
            status = AT_FAILURE;
        }
        //
        // Now check the table checksum
        // This just insures that the data we got makes some sense.
        //
		// Internal checksum is only 2-bytes, so we look two-bytes in
        crcVal = CalcChecksum((uint32_t *)(DataBuf+2), size - 2);
        if ((crcVal & 0xffff) != be16_to_cpu( ((uint16_t *)DataBuf)[0] ) )
        {
            status = AT_FAILURE;
        }
    }
    //
    // Now put the table onto the nvram where it is stored.
    if (status == AT_SUCCESS)
    {
        /*
         * Find our partition using the device search list from the NVM config layer.
         */
        searchList = getRommgrSearchList();
        targetDev  = SearchForPartition(0, CONFIG_SECTOR, searchList);
        if (targetDev == MEM_DEVICE_NOT_FOUND)
        {
            status = AT_FAILURE;        // the partition does not exist.
        }
        else
        {
            /* Try and open the partition. The open call will fail if the partition
             * is not found on the specified device.
             */
            if (PARTMGR_OK != OpenPartition(0, CONFIG_SECTOR, targetDev, &Handle))
            {
                status = AT_FAILURE;
            }
            else
            {
                /* Get partition information (we need the size to alloc memory) */
                // We have to make sure that the partition will hold what we are putting there.
                if (PARTMGR_OK != GetPartitionInfoByHandle(Handle, &Info))
                {
                    ClosePartition(Handle);
                    status = AT_FAILURE;
                }
                else
                {
                    if (Info.Size < size)   // make sure we have enough room for the data.
                    {
                        status = AT_FAILURE;
                    }
                    else
                    {
                        // finally erase and write the data to the partition.
                        //
                        if (PARTMGR_OK != ErasePartition(Handle, 0, size))
                        {
                            ClosePartition(Handle);
                            status = AT_FAILURE;
                        }
                        else
                        {
                            // now the write.
                            if (PARTMGR_OK != WritePartition(Handle, (int8_t *)DataBuf, 0, size, &length))
                            {
                                status = AT_FAILURE;
                            }
                            ClosePartition(Handle);
                        }
                    }
                }
            }
        }
    }
    //
    // Set the response to the command.
    //
    if (status != AT_SUCCESS)
    {
        Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);
    }
    //
    // Now send the response back to the host.
    //
    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);

    if ( NULL != DataBuf )
    {
        // free the buffer.
        MEM_FREE_AND_NULL(DataBuf);
    }

    return OK;
}

