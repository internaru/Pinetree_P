/***********************************************************
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* ==========================================================
*/

#include "boardCfg.h"
#include "dprintf.h"

#include "regDefs.h"
#include "nvram.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "agMemory.h"

#include "agiLogger.h"
#include "aclAPI.h"
#include "aclproto.h"


#define EXTERNAL_MEM_SIGNATURE  0xAA554154  ///< if present and containing compressed ELF images,

/*
 * Burn only code part
 */
ATSTATUS ACLBurnFlashCodePart(UINT8* dataBuf, UINT32 dataLen, UINT16 destination)
{
    int i;
    MESSAGE msg;
#define dbg1(x)    DPRINTF( (DBG_SOFT | DBG_OUTPUT), x )

    dbg1(("%s %d dataLen=%d\n", __FUNCTION__, __LINE__, dataLen ));
    DPRINTF((DBG_OUTPUT|DBG_SOFT),("%s\n", __FUNCTION__));

#if USING_8M_FLASH
    if(dataLen > (BOOTSPI_CODESECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE_2))
#else
    if(dataLen > BOOTSPI_CODESECTOR_SIZE)
#endif
    {
        dbg1(("%s %d %d > %d \n", __FUNCTION__, __LINE__, dataLen,
                    BOOTSPI_CODESECTOR_SIZE ));
        return AT_FAILURE;
    }
    dbg1(("%s %d\n", __FUNCTION__, __LINE__));

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: CodeSector\r\n"));
    //
    // Check to make sure this is a code sector.
    //
    if(((UINT32 *)dataBuf)[0] != EXTERNAL_MEM_SIGNATURE)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Error not code sector\n"));
        return AT_FAILURE;
    }
    if(!(destination & BF_NO_ERASE))
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erasing codesectors\r\n"));
        for(i = 0; i < BOOTSPI_NUM_CODESECTORS; i++)
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erasing codesector %d\r\n", i));
            eraseSectorFLASH(0, BOOTSPI_CODESECTOR_START + (BOOTSPI_SECTOR_SIZE*i));
        }

        if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, NULL, BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erase failed.\r\n"));
            return AT_FAILURE;
        }

#if USING_8M_FLASH
        //Erase 2nd Flash
         if(dataLen > (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE))
        {
            eraseBulkFLASH(1);
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2, NULL, BOOTSPI_CODESECTOR_SIZE_2) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erase failed at 2nd Flash.\r\n"));
                return AT_FAILURE;
            }
        }
#endif
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Burning codesectors\r\n"));
    // update the status.

#if USING_8M_FLASH
    if(dataLen <= BOOTSPI_CODESECTOR_SIZE)
    {
#endif

       blockWrFLASH(0,(unsigned int)BOOTSPI_CODESECTOR_START, dataBuf, dataLen);
       DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Finished write of codesector memory\n"));

       if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, dataBuf, dataLen) != AT_SUCCESS)
       {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed.\n"));
            return AT_FAILURE;
       }
#if USING_8M_FLASH
    }
    else
    {   //Write first BOOTSPI_CODESECTOR_SIZE code to 1st flash
        blockWrFLASH(0, (unsigned int)BOOTSPI_CODESECTOR_START, dataBuf, BOOTSPI_CODESECTOR_SIZE);
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Finished write of codesector memory at 1st flash\n"));
        if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, dataBuf, BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed at 1st flash.\n"));
            return AT_FAILURE;
        }
        //Write what is left to  2nd flash
        blockWrFLASH(1, (unsigned int)BOOTSPI_CODESECTOR_START_2, dataBuf + BOOTSPI_CODESECTOR_SIZE, dataLen - BOOTSPI_CODESECTOR_SIZE);
        if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2, dataBuf + BOOTSPI_CODESECTOR_SIZE, dataLen - BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed at 2nd flash.\n"));
            return AT_FAILURE;
        }
    }
#endif

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: done.\n"));

    return AT_SUCCESS;

}

