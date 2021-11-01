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
 *
 * \file readeeprom.c
 *
 * \brief Read the eeprom and return it to the host.
 *
 */

#include <string.h>
#include <stdint.h>

#include "dprintf.h"

#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "acl_api.h"
#include "regutils.h"
#include "checksum.h"
#include "rommgrapi.h"
#include "acl_nvram_constants.h"
#include "ioutils.h"
#include "platform.h"       // included for optional define of >> READ_EEPROM_TRUNCATE
#define MAX_CONFIG_TABLE_SIZE 2048

#define ACLCMD_READ_EEPROM         0x0003


#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    UINT32 readOffset;
    UINT32 readLength;
    uint8_t temp[4];
}readEEPROMCmd_t;


typedef struct  // 16 bytes
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    UINT32 numBytesRead;
    uint8_t temp[6];
} readEEPROMResp_t;

#define CONFIG_SECTOR     1
#pragma pack()
/* FUNCTION NAME: AclReadEEPROM */

/**
 * \brief Read the value of the eeprom and send it to the host.
 *
 * \param[in] Pipe  IO Pipe for reporting results to the host
 *
 * \param[in] AclCmd The command block that caused this to be called.
 *
 * \param reserved Save for later
 *
 * \retval SINT32 0 success, 1 failure.
 *
 * \date 2/27/2006
 *
 **/
int32_t AclReadEEPROM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    uint8_t    * pBufferMem = NULL;
    int8_t     *tempMem = NULL;
    ATSTATUS   status = AT_SUCCESS;
    uint32_t   crcVal;
    int32_t    bytesWritten;
    readEEPROMResp_t *Buffer;
    uint32_t offset, length;

    uint32_t   bytesRead = 0;

    Buffer = (readEEPROMResp_t *) construct_acl_response_buffer(ACLCMD_READ_EEPROM);

    offset = be32_to_cpu(((readEEPROMCmd_t *)AclCmd)->readOffset);
    length = be32_to_cpu(((readEEPROMCmd_t *)AclCmd)->readLength);
    if ( (offset > MAX_CONFIG_TABLE_SIZE )
         || (( offset + length) > MAX_CONFIG_TABLE_SIZE ) )
    {
        status = AT_FAILURE;
    }
    else
    {
        pBufferMem = (uint8_t *)MEM_MALLOC_ALIGN( length + CHECKSUM_LENGTH,
                                                  cpu_get_dcache_line_size() );
        if (!pBufferMem)
        {
            status = AT_FAILURE;
        }
        else
        {

            tempMem = RomMgrReadData(CONFIG_SECTOR, 0, &bytesRead); // get the config sector.
            if (NULL == tempMem)
            {
                MEM_FREE_AND_NULL(pBufferMem);
                status = AT_FAILURE;
            }
            else
            {
#ifdef HAVE_NAND_NV
                // static NV image of 512B is stored in NAND page of 2KB
                // when we read CONFIG_SECTOR, only use first 'length'
                bytesRead = (length < bytesRead)? length:bytesRead;
#endif
                // put the data into the send buffer.
                if (bytesRead > (uint16_t)length)
                {
                    status = cpu_to_be16(AT_FAILURE);
                }
                else
                {
                    memcpy(pBufferMem,tempMem,bytesRead);
                }
                MEM_FREE_AND_NULL(tempMem);  // done with this, free it.
            }
        }

    }

    // setup the ACL response
    if (status == AT_SUCCESS)
    {
        Buffer->numBytesRead = cpu_to_be32(bytesRead + CHECKSUM_LENGTH);

        // checksum the buffer
        crcVal = cpu_to_be32(CalcChecksum ((uint32_t*)pBufferMem, (uint16_t)bytesRead));

        // add the CRC to the end of the read data
        memcpy (pBufferMem + (uint16_t)bytesRead, (uint8_t*)&crcVal, CHECKSUM_LENGTH);
    }
    else
    {
        Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);
    }

    // Send the response.  We want to send the 16 byte portion of
    // the response EVEN WHEN status != AT_SUCCESS so that the
    // outside world knows that the command has been received.

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);

    // if sending success response, now send the data
    if (status == AT_SUCCESS)
    {
        // Note that we must send WriteConnect() a dynamically
        // allocated buffer.  WriteConnect() then frees that memory.
        bytesWritten = Pipe->WriteConnect(
                                         Pipe,
                                         pBufferMem,
                                         bytesRead + CHECKSUM_LENGTH);

        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACL: Sent %d bytes\n", bytesWritten));
    }
    else
    {
        if ( pBufferMem != NULL )
        {
            MEM_FREE_AND_NULL( pBufferMem );
        }
    }
    return OK;

}

