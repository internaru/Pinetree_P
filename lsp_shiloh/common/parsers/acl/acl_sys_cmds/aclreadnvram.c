/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file aclreadnvram.c
 * \brief Return the config table to the host.
*/

#include <string.h>

#include "lassert.h"
#include "dprintf.h"
#include "agRouter.h"
#include "nvram.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
  
#include "agiLogger.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "debug.h"
#include "aclAPI.h"



/* FUNCTION NAME: AclReadNVRAM */

/**
 * \brief Read the config table and return it to the host.
 *
 * \param[in] Pipe The io pipe from which to get the data.
 *
 * \param[in] AclCmd The acl command block
 *
 * \param reserved
 *
 * \retval 0 success, -1 failure
 *
 * \date 11/9/2006
 *
 **/
SINT32 AclReadNVRAM(IOReg *Pipe, ACL_CMD *AclCmd, void *reserved)
{
    UINT32 size, totalBytes,  bytesWritten;
    UINT32 status = AT_SUCCESS;
    SINT32 bytesRead, result;
    char *cmdStr = 0;
    char *pOutData = 0;

//    eDSVar myVarEnum;
    ACL_RESPONSE Buffer;
  //  tDSVarData myCopy;

    InitACLResponse(&Buffer);

    size = AclCmd->data.readNvram.strLen;

    cmdStr = (char *)MEM_MALLOC(size);
    ASSERT(cmdStr != NULL);

    totalBytes = 0;

    while (totalBytes < size)
    {
        bytesRead = Pipe->ReadConnect(Pipe, cmdStr, size, PARSEREAD_EXACT);
        if (bytesRead <= 0)
        {
            // We had a problem reading, chuck the data and return.

            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ReadConnect() failed and returned %d\n", bytesRead));
            goto ACLFAIL;        // FAIL calls agFree()
        }
        totalBytes += bytesRead;
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: AclReadNvram() reading: %s\n", cmdStr));

    // Now get the enum for the data store value described by the input string
 //   result = GetVarEnumFromString( cmdStr );

    if (-1 == result)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: AclReadNvram() enumeration not in table!\n"));
        Buffer.cmdStatus = AT_FAILURE;
        status = AT_FAILURE;
    }
    else
    {
 //       myVarEnum = (eDSVar)result;

        // Get the variable value itself and send it back.
  //      size = DSVarSize( myVarEnum );

   //     myCopy.name = myVarEnum;

        pOutData = (char *)MEM_MALLOC_ALIGN(size, cpu_get_dcache_line_size());
        ASSERT(pOutData != NULL);
       // myCopy.pVar = (void *)pOutData;

 //       memcpy((void *)pOutData,(void *)DSGetROVar(myCopy.name), size);
       // DSGetVar( &myCopy );
       // DSUnlockVar(myVarEnum);
    }

    // Send the response.  We want to send the 16 byte portion of
    // the response EVEN WHEN status != AT_SUCCESS so that the
    // outside world knows that the command has been received.
    // InitACLResponse() takes care of filling in prefix and status fields.
    Buffer.cmdID = ACLCMD_READ_NVRAM;
    Buffer.data.nvramWriteResp.responseLen = size;

    bytesWritten = Send16ByteACLResponse(Pipe, &Buffer);
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", bytesWritten));

    // Send the data
    if ( status == AT_SUCCESS )
    {
        bytesWritten = Pipe->WriteConnect( Pipe, pOutData, size);
        pOutData = 0; // WriteConnect() side effect is to free pOutData

        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", bytesWritten));
    }

    MEM_FREE_AND_NULL(cmdStr);
    if ( NULL != pOutData )
    {
        MEM_FREE_AND_NULL( pOutData );
    }
    return 0;

    ACLFAIL:
        MEM_FREE_AND_NULL(cmdStr);
        if ( NULL != pOutData )
        {
            MEM_FREE_AND_NULL( pOutData );
        }
        return -1;

}

