/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file aclreadtable.c
 * \brief Read any memory location in the system.
*/

#include <string.h>

#include "dprintf.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "acl_api.h"
#include "checksum.h"
#include "ioutils.h"

#define MAX_LENGTH  512*100     // 100 blocks of 512 bytes each.


#define ACLCMD_READ_TABLE          0x0009


#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    uint32_t numBytesRead;
    uint8_t temp[8];
} readTableResp_t;


typedef struct  // 16 bytes
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    uint32_t readOffset;
    uint32_t readLength;
    uint8_t  temp[2];
}readTablecmd_t;

#pragma pack()
/* FUNCTION NAME: AclReadTable */

/**
 * \brief Read a section of memory and return it to the host.
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
error_type_t AclReadTable(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    ATSTATUS   status = AT_SUCCESS;
    UINT8    * pBufferMem = NULL;
    SINT32    bytesWritten, TotalBytes;
    readTableResp_t *Buffer;
    uint32_t read_length, read_offset;

    Buffer = (readTableResp_t *)construct_acl_response_buffer(ACLCMD_READ_TABLE);
    // Read requested bytes to a buffer.
    read_offset = be32_to_cpu(((readTablecmd_t *) AclCmd)->readOffset);
    read_length = be32_to_cpu(((readTablecmd_t *) AclCmd)->readLength);
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ReadTable start addr 0x%x, nBytes = %u\n", 
                    read_offset, read_length ));

    if (read_offset == 0)
    {
        //if user sends in a 0, dump the thread list
        DPRINTF( (DBG_SOFT | DBG_OUTPUT), ("Use 'USBCmd -c threads' to see threads from host or simply 'threads' from cmd line\n") );
    }

    // Setup the ACL response.

    if ( status == AT_SUCCESS )
    {
        //Buffer->numBytesRead = ((readTablecmd_t *)AclCmd)->readLength;
        memcpy(&(Buffer->numBytesRead),&(((readTablecmd_t *)AclCmd)->readLength), 4);

    }

    // Send the response.  We want to send the 16 byte portion of
    // the response EVEN WHEN status != AT_SUCCESS so that the
    // outside world knows that the command has been received.

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);  // send the response

    TotalBytes = 0;
    while(read_length > TotalBytes)
    {
        // Note that we must send WriteConnect() a dynamically
        // allocated buffer. WriteConnect() then frees that memory.
        bytesWritten = read_length - TotalBytes;

        if(bytesWritten > MAX_LENGTH)   // limit the size of the xfer.
            bytesWritten = MAX_LENGTH;

		DPRINTF((DBG_SOFT | DBG_OUTPUT),("ReadTable memcpy start\n"));
        while((pBufferMem = (UINT8 *)MEM_MALLOC( bytesWritten )) == NULL)
            sleep(1);

        {
            memcpy( pBufferMem,
                    (void *)(read_offset + TotalBytes),
                    bytesWritten );
			DPRINTF((DBG_SOFT | DBG_OUTPUT),("ReadTable memcpy complete\n"));
        }
        bytesWritten = Pipe->WriteConnect(Pipe,
                                         pBufferMem,
                                         bytesWritten);
        TotalBytes += bytesWritten;
    }


     return OK;
}

