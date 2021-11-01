/***********************************************************
* (c) Copyright 2005-2008 Marvell International Ltd.
*
*               Marvell Confidential
* ==========================================================
*/

#include "dprintf.h"

#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"

/* FUNCTION NAME: VerifyFlash */

/**
 * \brief Verifies that the flashBuf is identical to the srcBuf.
 * If srcBuf == NULL then the function checks to see if the flash
 * specified by flashBuf is erased.
 *
 * NOTE that this function does not call memcmp or any other cruntime
 * function.  This is because we want the function to be self-contained
 * to verify that it runs entirely out of DRAM, not out of the flash
 * that may now be in an unknown state.
 *
 * \param flashBuf
 *
 * \param srcBuf
 *
 * \param size
 *
 * \retval ATSTATUS AT_SUCCESS if verify succeeds; AT_FAILURE else
 *
 * \author Jay Shoen
 *
 * \date 10/29/2005
 *
 **/
ATSTATUS VerifyFlash(UINT8* flashBuf, UINT8* srcBuf, UINT32 size)
{
    UINT32 i;
    UINT32 offset = 0, numErr = 0;

    if(srcBuf == NULL)
    {
        // srcBuf == NULL indicates that we should verify that the flash is erased
        for(i = 0; i < size; i++)
        {
            if(flashBuf[i] != 0xFF)
            {
                if(offset == 0)
                {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLVerifyFlash: failed at offset: 0x%x, flash: 0x%x\r\n", i, flashBuf[i]));
                    offset = i;
                }
                numErr++;
            }
        }
    }
    else
    {
        for(i = 0; i < size; i++)
        {
            if(flashBuf[i] != srcBuf[i])
            {
                if(offset == 0)
                {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLVerifyFlash: failed at offset: 0x%x, src: 0x%x. flash: 0x%x\r\n", i, srcBuf[i], flashBuf[i]));
                    offset = i;
                }
                numErr++;
            }
        }
    }

    if(numErr != 0)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLVerifyFlash: failed num corrupt: %d\n", numErr));
        return AT_FAILURE;
    }
    else
        return AT_SUCCESS;
}

