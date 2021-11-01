/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file writenvram.c
 * \brief Write a nvram value to the dynamic section.
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
#include "dataStore.h"
#include "dsLocal.h"
#include "debug.h"
#include "aclAPI.h"



/* FUNCTION NAME: AclWriteNVRAM */

/**
 * \brief Write the datastore value given by the string parameter
 *
 * \param[in] Pipe The io pipe
 *
 * \param[in] AclCmd The command block
 *
 * \param reserved
 *
 * \retval SINT32 0 is success.
 *
 * \author David Bartle
 *
 * \date 11/20/2006
 *
 **/
SINT32 AclWriteNVRAM(IOReg *Pipe, ACL_CMD *AclCmd, void *reserved)
{
    UINT32 size, totalBytes,  bytesWritten;
    // UINT32 status = AT_SUCCESS;
    SINT32 bytesRead, result, retval = 0;
    char *cmdStr = 0, *pInData = 0, *pTempData = 0;

    eDSVar myVarEnum;
    ACL_RESPONSE Buffer;
    tDSVarData myCopy;

    InitACLResponse(&Buffer);

    size = AclCmd->data.writeNvram.strLen;

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
            retval = -1;
            goto ACLFAIL;
        }
        totalBytes += bytesRead;
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: AclReadNvram() writing: %s\n", cmdStr));

    // Now get the enum for the data store value described by the input string
    result = GetVarEnumFromString( cmdStr );
    if (-1 == result)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: AclWriteNvram() enumeration not in table!\n"));
        retval = -1;
        goto ACLFAIL;
    }

    myVarEnum = (eDSVar)result;

    // Read the rest of the data, which will be the contents of the
    // datastore variable to be written.
    size = AclCmd->data.writeNvram.writeLen;
    ASSERT(size == DSVarSize(myVarEnum));

    pInData = (char *)MEM_MALLOC( size );
    ASSERT(pInData != NULL);

    totalBytes = 0;

    while (totalBytes < size)
    {
        bytesRead = Pipe->ReadConnect(Pipe, pInData, size, PARSEREAD_EXACT);
        if (bytesRead <= 0)
        {
            // We had a problem reading, chuck the data and return.

            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ReadConnect() failed and returned %d\n", bytesRead));
            retval = -1;
            goto ACLFAIL;        // FAIL calls MEM_FREE_AND_NULL()
        }
        totalBytes += bytesRead;
    }

    // Does this enum represent a writeable datastore value? If not, fail!
    if ( e_ReadWrite != DSIsWriteable(myVarEnum) )
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: AclWriteNvram() enumerated value NOT WRITEABLE!\n"));
        retval = -1;
        goto ACLFAIL;
    }

    // Get the current data store value. This locks the variable semaphore
    // in preparation for setting the value.
    pTempData = (char *)MEM_MALLOC( size );
    ASSERT(pTempData != NULL);
    myCopy.name = myVarEnum;
    myCopy.pVar = (void *)pTempData;
    DSGetVar(&myCopy);

    myCopy.pVar = pInData;
    DSSetVar(&myCopy);
    MEM_FREE_AND_NULL(pTempData);

    // TESTING - get the value out of datastore and compare the value read
    /*
    myCopy.pVar = DSGetROVar(myVarEnum);

    for(i=0; i<size; i++)
        agprintf(">>> %x %x\n", *(char *)myCopy.pVar++, *(char *)pInData++);
    */

    ACLFAIL:

    // Send the response.  We want to send the 16 byte portion of
    // the response EVEN WHEN status != AT_SUCCESS so that the
    // outside world knows that the command has been received.
    // InitACLResponse() takes care of filling in prefix and status fields.
    Buffer.cmdID = ACLCMD_WRITE_NVRAM;
    Buffer.data.nvramWriteResp.responseLen = 0;

    bytesWritten = Send16ByteACLResponse(Pipe, &Buffer);
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", bytesWritten));

    if (NULL != cmdStr)
	{
		MEM_FREE_AND_NULL(cmdStr);
	}
    if (NULL != pInData)
	{
		MEM_FREE_AND_NULL(pInData);
	}
    return retval;
}

