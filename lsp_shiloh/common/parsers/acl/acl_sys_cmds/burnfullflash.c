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
 * Burn both bootcode and code sectors
 *
*/

ATSTATUS ACLBurnFlashFullPart(UINT8* dataBuf, UINT32 dataLen, UINT16 destination)
{
    int i;
    MESSAGE msg;
    DPRINTF((DBG_OUTPUT|DBG_SOFT),("%s\n", __FUNCTION__));

#if USING_8M_FLASH
    if(dataLen > (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE_2))
#else
    if(dataLen > (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE))
#endif
    {
       return AT_FAILURE ;
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart\r\n"));
    //
    // Make sure the first sector is boot.
    if(((UINT32 *)dataBuf)[0] == EXTERNAL_MEM_SIGNATURE)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Error Full burn, no boot sector. %d\n", __LINE__));
        return AT_FAILURE;
    }
    //
    // Now make sure the 2nd sector has code.
    //
    if(((UINT32 *)(dataBuf+BOOTSPI_BOOTSECTOR_SIZE))[0] != EXTERNAL_MEM_SIGNATURE)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Error no code sector\n"));
        return AT_FAILURE;
    }


    if(!(destination & BF_NO_ERASE))
    {
        //
        // See if we need to erase the boot sector.  If the version
        // of the boot code has not changed, only erase the code.
        //
        if(((UINT32 *)dataBuf)[0] == ((UINT32 *)BOOTSPI_BOOTSECTOR_START)[0])
        {
            // Erase only the code sector.
            for(i = 0; i < BOOTSPI_NUM_CODESECTORS; i++)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erasing codesector %d\r\n", i));
                eraseSectorFLASH(0, BOOTSPI_CODESECTOR_START+(BOOTSPI_SECTOR_SIZE*i));
            }

            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, NULL, BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erase failed at codesector.%d\n", __LINE__));
                return AT_FAILURE;
            }
        }
        else
        {
            // erase everything.
            //
            eraseBulkFLASH(0);
            if(VerifyFlash((UINT8*)BOOTSPI_BOOTSECTOR_START, NULL, BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erase failed at 1st Flash.%d\n", __LINE__));
                return AT_FAILURE;
            }

        }
#if USING_8M_FLASH
        if(dataLen > (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE))
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: To erase 2nd flash.\n"));
            eraseBulkFLASH(1);
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2, NULL, BOOTSPI_CODESECTOR_SIZE_2) != AT_SUCCESS)
            {
                 DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Erase failed with at Flash.%d\n", __LINE__));
             return AT_FAILURE;
            }
        }
#endif
    }

    //
    // See if we need to write the whole thing or just the code.
    // If the version is the same for the boot code, do not
    // write it.
    //
    if(((UINT32 *)dataBuf)[0] == ((UINT32 *)BOOTSPI_BOOTSECTOR_START)[0])
    {
        // Just write the code.
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Write Code part Length %d\n",dataLen-BOOTSPI_BOOTSECTOR_SIZE));
        //
        // Since the data buffer also contains the boot code, move the pointer
        // past the boot code and reduce the amount to write.
        //
        //blockWrFLASH(0,(unsigned int)BOOTSPI_CODESECTOR_START,
        //             dataBuf+BOOTSPI_BOOTSECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE);
        //
#if USING_8M_FLASH
       if( (dataLen - BOOTSPI_BOOTSECTOR_SIZE) <= BOOTSPI_CODESECTOR_SIZE) //See if we need to write to 2nd Flash
       {
#endif
            blockWrFLASH(0,(unsigned int)BOOTSPI_CODESECTOR_START,  dataBuf + BOOTSPI_BOOTSECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE);
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Finished write of codesector memory.%d\n", __LINE__));

            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START,  dataBuf + BOOTSPI_BOOTSECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed.%d\n", __LINE__));
                return AT_FAILURE;
            }
#if USING_8M_FLASH
        }
        else
        {   //Write BOOTSPI_CODESECTOR_SIZE code to 1st flash
             blockWrFLASH(0, (unsigned int)BOOTSPI_CODESECTOR_START,
                             dataBuf + BOOTSPI_BOOTSECTOR_SIZE,
                             BOOTSPI_CODESECTOR_SIZE);

             if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START,
                            dataBuf + BOOTSPI_BOOTSECTOR_SIZE,
                            BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
             {
                  DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed at 1st flash.%d\n", __LINE__));
                  return AT_FAILURE;
             }
             //Write what is left of code to 2nd flash
             blockWrFLASH(1, (unsigned int)BOOTSPI_CODESECTOR_START_2,
                             dataBuf + BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE,
                             dataLen - BOOTSPI_BOOTSECTOR_SIZE - BOOTSPI_CODESECTOR_SIZE);

             if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2,
                             dataBuf + BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE,
                             dataLen - BOOTSPI_BOOTSECTOR_SIZE- BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
             {
                  DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Codesector burn verification failed at 2nd flash.%d\n", __LINE__));
                  return AT_FAILURE;
             }
        }
#endif

    }
    else
    {//Write both boot and code
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Write full part Length %d\n",dataLen));
#if USING_8M_FLASH
        if( dataLen <= (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE)) //See if we need to write to 2nd Flash
        {  //Only need 1st flash
#endif
            blockWrFLASH(0,(unsigned int)BOOTSPI_BOOTSECTOR_START, dataBuf,dataLen );
#if USING_8M_FLASH
        }
        else
        {  //write to 1st flash
            blockWrFLASH(0, (unsigned int)BOOTSPI_BOOTSECTOR_START,
                             dataBuf,
                             BOOTSPI_BOOTSECTOR_SIZE+ BOOTSPI_CODESECTOR_SIZE );

            //Write what's left to 2nd flash
            blockWrFLASH(1, (unsigned int)BOOTSPI_CODESECTOR_START_2,
                            dataBuf + BOOTSPI_CODESECTOR_SIZE+ BOOTSPI_BOOTSECTOR_SIZE,
                            dataLen - BOOTSPI_CODESECTOR_SIZE- BOOTSPI_BOOTSECTOR_SIZE);
        }
#endif

    }

        //
        //  Only verify the parts we wrote. If we wrote the whole part check it
    //  If we only wrote the code part check only it. This is because the last
    //  part of the boot sector will have different values depending on how
    //  it was programmed because:
    //  1. If programmed by itself the part is erased and only the boot code size
    //     is written, leaving the last part of the sector with 0xFF's.
    //  2. If written combined with the code the utility apparently starts wiith a NULL'd
    //     buffer, adds the bootcode and then the code into a single image. When the whole
    //     part is programmed the last part of the bootsector is programmed to 0x00's
    //
    //  This means the verify at the end will only match the buffer if we programmed the
    //  entire part. If not it might not match even if all was okay.
    if(((UINT32 *)dataBuf)[0] == ((UINT32 *)BOOTSPI_BOOTSECTOR_START)[0])
    { //begin of verifing only the code part
#if USING_8M_FLASH
        if( dataLen <= (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE))
        {   //only check 1st flash
#endif
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, dataBuf+BOOTSPI_BOOTSECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed. %d\n", __LINE__));
                return AT_FAILURE;
            }
#if USING_8M_FLASH
        }
        else
        {  //need to verify both flashes
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, dataBuf + BOOTSPI_BOOTSECTOR_SIZE, BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed. %d\n", __LINE__));
                return AT_FAILURE;
            }
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2, dataBuf+BOOTSPI_BOOTSECTOR_SIZE+BOOTSPI_CODESECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE-BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed at 2nd Flash. %d\n", __LINE__));
                return AT_FAILURE;
            }

        }
#endif

    } //end of verifing only code
    else
    { //begin of verifing both boot and code parts
#if USING_8M_FLASH
        if( dataLen <= (BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE))
        { //verify only 1st flash
#endif
            if(VerifyFlash((UINT8*)BOOTSPI_BOOTSECTOR_START, dataBuf, dataLen) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed. %d\n", __LINE__));
            return AT_FAILURE;
            }
#if USING_8M_FLASH
        }
        else
        {//verify both flashes
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START, dataBuf, BOOTSPI_BOOTSECTOR_SIZE + BOOTSPI_CODESECTOR_SIZE ) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed. %d \n", __LINE__));
                return AT_FAILURE;
            }
            if(VerifyFlash((UINT8*)BOOTSPI_CODESECTOR_START_2, dataBuf+BOOTSPI_BOOTSECTOR_SIZE+BOOTSPI_CODESECTOR_SIZE, dataLen-BOOTSPI_BOOTSECTOR_SIZE-BOOTSPI_CODESECTOR_SIZE) != AT_SUCCESS)
            {
                DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: FullPart burn verification failed at 2nd Flash. %d\n",__LINE__));
                return AT_FAILURE;
            }
        }
#endif
    }//end of verifing both boot and code parts

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACLBurnFlash: Burn full part done.\n"));

    return AT_SUCCESS;

}

