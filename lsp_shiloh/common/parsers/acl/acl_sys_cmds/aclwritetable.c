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
 * \file aclwritetable.c
 *
 * \brief Write data to a specific location in memory.
 *
 */


#include <string.h>
#include <stdint.h>

#include "dprintf.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "logger.h"
#include "memAPI.h"
#include "acl_api.h"
#include "checksum.h"
#include "acl_nvram_constants.h"
#include "ioutils.h"


#define ACLCMD_WRITE_TABLE         0x000A

#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    UINT32 writeOffset;
    UINT32 writeLength;
    uint8_t temp[4];
}writeTableCmd_t;

#pragma pack()

/* FUNCTION NAME: AclWriteTable */

/**
 * \brief Take the next data and write it to memory.
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
error_type_t AclWriteTable(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    ATSTATUS status=AT_SUCCESS;
    intptr_t     offset;
    uint32_t     size;
    SINT32 bytesRead=0;
    acl_response_base_struct_t *Buffer;
    uint8_t checksum[CHECKSUM_LENGTH];

    Buffer = construct_acl_response_buffer(ACLCMD_WRITE_TABLE);

    size = be32_to_cpu(((writeTableCmd_t *)AclCmd)->writeLength);
    offset = be32_to_cpu(((writeTableCmd_t *)AclCmd)->writeOffset);

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Write Table of %u bytes, offset %x\n", (size - CHECKSUM_LENGTH), offset));

    // read in everything but the checksum to the target location
    bytesRead = Pipe->ReadConnect(Pipe, (void*)offset, (size - CHECKSUM_LENGTH), PARSEREAD_EXACT);
    if(bytesRead != (size - CHECKSUM_LENGTH))
    {
        status = AT_FAILURE;
    }
    else
    {
        // now read in the checksum (this is just thrown away)
        bytesRead = Pipe->ReadConnect(Pipe, checksum, CHECKSUM_LENGTH, PARSEREAD_EXACT);
        if(bytesRead != CHECKSUM_LENGTH)
        {
            status = AT_FAILURE;
        }
    }

    if ( status != AT_SUCCESS )
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Wrong number of bytes read, abort\n"));
        Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);
    }

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent write bytes\n"));
    return OK;
}

