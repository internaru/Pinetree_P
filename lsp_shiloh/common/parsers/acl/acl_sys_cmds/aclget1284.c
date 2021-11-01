/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file aclget1284.c
 * \brief Return the 1284 string.
*/

#include <string.h>

#include "lassert.h"
#include "dprintf.h"
#include "memAPI.h"

#include "logger.h"
#include "debug.h"
#include "acl_api.h"
#include "platform_api.h"
#include "ioutils.h"

#define ACLCMD_GET_1284_STRING     0x0006

#pragma pack(1)
typedef struct
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    uint16_t responseLen;
    char temp[8];
} acl_1284_response_t;

#pragma pack()
/* FUNCTION NAME: AclGet1284String */

/**
 * \brief Return the USB 1284 string to the host.
 * The 1284 string is defined in the usb spec for printers.  This command
 * returns the string to the host.
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
error_type_t AclGet1284String(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
    ATSTATUS  status     = AT_SUCCESS;
    uint8_t   * pBufferMem = NULL;
    int32_t bytesWritten = 0; UNUSED_VAR(bytesWritten); 
    int32_t length;
    acl_1284_response_t *Buffer;
    char *ACLDeviceId;

    Buffer = (acl_1284_response_t *)construct_acl_response_buffer(ACLCMD_GET_1284_STRING);
    platvars_get_1284str(&ACLDeviceId);
    // Note that we must send WriteConnect() a dynamically
    // allocated buffer. WriteConnect() then frees that memory.
    length = be16_to_cpu(*(UINT16*)&ACLDeviceId[0]);
    pBufferMem = (uint8_t *)MEM_MALLOC( length );

    Buffer->responseLen = cpu_to_be16(length);

    if (pBufferMem == NULL)
    {
        status = AT_FAILURE;
    }
    else
    {
        memcpy( pBufferMem, ACLDeviceId, length );
    }

    // Send the response header

    if (status != AT_SUCCESS)
    {
        Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);
    }
    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);
    // everything in buffer is invalid at this point.
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent 1284 string\n"));

    // Send the string itself

    if ( status == AT_SUCCESS )
    {
        bytesWritten = Pipe->WriteConnect(
                                         Pipe,
                                         pBufferMem,
                                         length);

        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", bytesWritten));
    }
    else
    {
        if ( pBufferMem != NULL )
        {
            MEM_FREE_AND_NULL( pBufferMem );
        }
    }
    return 0;
}
/*
 * End of aclget1284.h
 */

