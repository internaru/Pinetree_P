/**
* (c) Copyright 2006-2008,2014 Marvell International Ltd.
*
*               Marvell Confidential
* 
 * \file aclgetbuildinfo.c
 * \brief Returns the build info to the host.
*/

#include <string.h>

#include "lassert.h"
#include "dprintf.h"
#include "ATypes.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "acl_api.h"
#include "ioutils.h"
#include "fwVersion.h"

#ifdef HAVE_NETWORK
#include "net_iface_vars.h"
#ifdef __linux__
#include <net/if.h>
#else
#include "cpu_api.h"
#include "trsocket.h"
#endif
#include "pthread.h"
#include "posix_ostools.h"
#include "platform_api.h"
#endif // defined(HAVE_NETWORK)

#ifndef __linux__
#include "cpu_api.h"
#endif

#define ACLCMD_GET_BUILD_INFO 0x0008


#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    uint16_t responseLen;
    uint8_t temp[8];
} getBuildInfoCmd_t;

#pragma pack(0)
/* FUNCTION NAME: AclGetBuildInfo */

/**
 * \brief Return the fw build info to the host.
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
error_type_t AclGetBuildInfo(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
    ATSTATUS   status     = AT_SUCCESS;
    UINT8    * pBufferMem = NULL;
    UINT       nBytes     = strlen( BUILD_INFO_STRING );
    SINT32     bytesWritten  = 0; UNUSED_VAR(bytesWritten); 
    getBuildInfoCmd_t *Buffer;
#if defined(HAVE_NETWORK)
    char	ipAddress[25] = { 0 };

    nBytes += sizeof(ipAddress);
#endif

    Buffer = (getBuildInfoCmd_t *) construct_acl_response_buffer(be16_to_cpu(AclCmd->cmd_id));

    // Note that we must send WriteConnect() a dynamically
    // allocated buffer. WriteConnect() then frees that memory.

    pBufferMem = (UINT8 *)MEM_MALLOC( nBytes + 1 );

    if ( pBufferMem == NULL )
    {
        status = AT_FAILURE;
    }
    else
    {
        memcpy( pBufferMem, BUILD_INFO_STRING, nBytes );
        pBufferMem[nBytes] = 0;  // just so we can dprintf() it as a string
    }

#if defined(HAVE_NETWORK)
#if defined(WIRED_NETWORK)
    if (pBufferMem != NULL)
    {
        char     *phys_addr;
        uint32_t ip_addr;
        int       i;
        char      zero_test = 0;

        platvars_get_mac_addr(&phys_addr);
        for (i = 0; i < NET_IFACE_PHYS_ADDR_SIZE; i++)
        {
            zero_test |= phys_addr[i];
        }

        if (zero_test != 0)
        {
            // TODO must account for multiple simultaneous interfaces
            unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

            net_iface_get_var_ipv4_addr(if_index, &ip_addr);
            ip_addr = ntohl(ip_addr);
            minSprintf(ipAddress, " IP@%d.%d.%d.%d\r\n", 
                       (int)((ip_addr >> 24) & 0xFF), (int)((ip_addr >> 16) & 0xFF),
                       (int)((ip_addr >> 8)  & 0xFF), (int)(ip_addr & 0xFF));
        }
        else
        {
            minSprintf(ipAddress, " [NO MAC ADDRESS]\r\n");
        }
	    strcat((char *)pBufferMem, ipAddress);
    }
#endif
#endif

    // Send the response header.

    if (status != AT_SUCCESS)
    {
        Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);
        Buffer->responseLen = 0;
    }
    else
    {
        Buffer->responseLen = cpu_to_be16(nBytes);
    }

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);      // non-blocking write.

    // Send the string itself.

    if ( status == AT_SUCCESS )
    {
        bytesWritten = Pipe->WriteConnect(Pipe, pBufferMem, nBytes);

        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes: %s\n", bytesWritten, pBufferMem));
    }
    else
    {
        if ( pBufferMem != NULL )
            MEM_FREE_AND_NULL( pBufferMem );
    }
    return OK;
}

