/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file globburnflash.c
 *
 */

#if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)
#include <stdint.h>

#include "error_types.h"

#ifdef HAVE_BOOTSPI
#include "bootspi_api.h"
#endif

#ifdef HAVE_NAND
#include "nand_burn_flash.h"
#endif

#include "ACL_burn_flash_event_api.h"

#include "deviceid.h"
#include "acl_api.h"
#include "logger.h"
#include "lassert.h"
#include "checksum.h"
#ifdef HAVE_EEPROM
#include "eeprom_api.h"
#endif
#include "acl_nvram_constants.h"

#if 0
//#ifdef WIRELESS
#include "wfwuploader.h"    //-jrs
#endif


#define CODE_KEY            0xC0DEED0C   /**< ROM Code Table signature does not belong here BUGBUG */

#if HAVE_BOOTSPI
error_type_t burni2c(uint8_t *dataBuf, uint32_t dataLen, uint32_t offset);
error_type_t burnFlash(uint32_t SpiCs, uint8_t *dataBuf, uint32_t dataLen, uint32_t offset);
#endif



/*
** ACLBurnFlash
 *
 *  PARAMETERS:
 *      dataBuf - pointer to buffer containing the data to burn
 *      dataLen - len in bytes of dataBuf
 *      offset -  offset in the part at which to start burning the data; some destinations may support only offsets of 0
 *      destination - destination code of burn target.  Must be one of the BF_... defines from above
 *      checkSum - 16bit folded checksum of the dataBuf
 *
 *  DESCRIPTION:
 *      This routine will burn the dataBuf passed to the specified destination.  The checksum will be checked
 *      before the burn is done.
 *
 *  RETURNS: On success: OK; else FAIL.
 *
 */
error_type_t ACLBurnFlash(uint8_t* dataBuf, uint32_t dataLen, uint32_t offset, uint16_t destination, uint16_t checkSum)
{
    uint16_t checkVal;
    error_type_t status;

    status = FAIL;

    DPRINTF((DBG_ENTRY | DBG_OUTPUT),("ACLBurnFlash dataLen = %d, offset = %d\n", dataLen, offset));

    checkVal = (uint16_t)CalcChecksum((uint32_t *) dataBuf, dataLen);
    if(checkVal != checkSum)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ACLBurnFlash: checksum failed.  Should be %#x, is %#x\n", checkSum, checkVal));
        goto EXIT;
    }

    switch(destination & (~BF_MASK))    // mask off the non destination bits when determining destination
    {

#if HAVE_BOOTSPI
		case BF_SPI0:
        case BF_SPI1:
        case BF_SPI2:
        case BF_SPI3:
            status = burnFlash(destination & (~BF_NO_ERASE), dataBuf, dataLen, offset);
            break;

        case BF_I2C:
            status = burni2c(dataBuf, dataLen, offset);
            break;
#endif

#ifdef HAVE_NAND		
		case BF_NAND0:
			status= nand_burn_flash(MEM_NAND0,dataBuf,dataLen);
			break;
		case BF_NAND1:
			status= nand_burn_flash(MEM_NAND1,dataBuf,dataLen);
			break;
#endif

#if 0
//#ifdef WIRELESS
        case BF_MARVELL_WIRELESS:
            {
                uint32_t Radio;
                Radio = DSGetIntVar(e_WirelessRadioPresent);
                if(Radio != WB_NOT_INSTALLED)
                {
                    // call the routine to burn the wireless flash
                    if(WirelessRescue(dataBuf, dataLen) == 0)
                    {
                        status = OK;
                    }
                    else
                    {
                        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ACLBurnFlash: ERROR: WirelessRescue failed.\n"));
                    }
                }
            }
            break;
#endif  // WIRELESS

        default:
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("ACLBurnFlash: ERROR: invalid destination %#x.\n", destination));
            break;
    }
    // if the CMD was SUCCESS and it is NON REBOOT-able
    if ((status  == AT_SUCCESS) && (destination & BF_NO_REBOOT)) {
        status = AT_PENDING;
    }

EXIT:
    DPRINTF((DBG_ENTRY | DBG_OUTPUT),("<==ACLBurnFlash status = %#x\n", status));
    return status;
}

#if HAVE_BOOTSPI
error_type_t burni2c(uint8_t *dataBuf, uint32_t dataLen, uint32_t offset)
{
#ifdef HAVE_EEPROM
    EEPROM_DEVICE_t *Handle;
    error_type_t status = OK;
    uint64_t myOffset = offset;

    status = eepromOpen(MEM_I2C, &Handle);
    if(status == OK)
    {
        status = eepromWrite(Handle, dataBuf, myOffset, dataLen);
        if(status == dataLen)
            status = OK;
        else
            status = FAIL;
        eepromClose(Handle);
    }
    return status;
#else
	return FAIL;
#endif
}
error_type_t burnFlash(uint32_t SpiCs, uint8_t *dataBuf, uint32_t dataLen, uint32_t offset)
{
    spi_device_t *Handle;
    error_type_t status;
    MEMORY_DEVICE Part = MEM_DEVICE_NOT_FOUND;
    DRIVER_CONFIG_t spiInfo;
    uint32_t currentPartAddress=offset;
    int32_t codeTableBlock = -1;    // no code table found.
    int32_t codeTableLen = 0;    // no code table found.
    int32_t codeTableBufPtr = 0;    // no code table found.
    uint32_t BlockCount;
    uint32_t BlockAddr;
    uint32_t writeLen;
    uint32_t curDataPtr = 0;
    uint32_t dataLeftToWrite = dataLen;
    

    switch(SpiCs)
    {
        case BF_SPI0:
            Part = MEM_SPI0;
            break;
        case BF_SPI1:
            Part = MEM_SPI1;
            break;
        case BF_SPI2:
            Part = MEM_SPI2;
            break;
        case BF_SPI3:
            Part = MEM_SPI3;
            break;
        default:
            ASSERT(0);
    }
    status = bspi_open(Part, &Handle);
    ASSERT(status == OK);

    bspi_report(Handle, &spiInfo);
    
    /*
        Now we need to start burning the part.  This will be done in pieces.  I check
        to see if the block I am burning starts with the codetable.  If it does I save 
        that block for last.  Then I burn all the other blocks and do the code table.
        I burn the blocks by erasing a single block and then burning that block.  That
        reduces the "brick" time to as short as possible.
    */
    for(BlockCount = 0; BlockCount < spiInfo.BlockCount && dataLeftToWrite > 0; BlockCount++)
    {
        BlockAddr = BlockCount * spiInfo.BlockSize; // address of the start of this block
        if((BlockAddr <= currentPartAddress) &&
           ((BlockAddr + spiInfo.BlockSize) > currentPartAddress))
        {
            //
            // See if this has the code table
            // 
            if((currentPartAddress == BlockAddr) &&   // are we writing from the start of the block?
               (*((uint32_t *)(&dataBuf[BlockAddr])) == CODE_KEY))
            {
                DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Found a code table in block %d\n",BlockCount));
                // Code table is here.  Save to write later.
                codeTableBlock = BlockCount;
                codeTableBufPtr = curDataPtr;
                codeTableLen = spiInfo.BlockSize;
                if(codeTableLen > dataLeftToWrite)
                {
                    codeTableLen = dataLeftToWrite;
                }
                dataLeftToWrite -= codeTableLen;        // amount left to do.
                curDataPtr += codeTableLen;
                currentPartAddress += codeTableLen;
                continue;       // go to the next for loop.
            }
            //
            // Figure out how much to write.
            // 
            writeLen = BlockAddr + spiInfo.BlockSize - currentPartAddress;
            if(writeLen > dataLeftToWrite)
            {
                writeLen = dataLeftToWrite;
            }
            
            //
            // this block has something in it.  see if this is a partial block.
            // BUGBUG, do this later.  Not used currently.
            //
            // erase the sector.
            //
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Erase block %d\n",BlockCount));
            bspi_erase_sector(Handle, BlockCount);

            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("WriteBlock %x length %d\n",currentPartAddress, writeLen));
            status = bspi_write(Handle, (int8_t *)&dataBuf[curDataPtr], (uint64_t)currentPartAddress, writeLen);
            ASSERT(status == writeLen);
            // 
            // update the pointers
            //
            curDataPtr += writeLen;
            currentPartAddress += writeLen;
            dataLeftToWrite -= writeLen;

        }
        
    }
    //
    // now write the code table
    //
    if(codeTableBlock != -1)
    {
        // we have a code table to write.
        //
        bspi_erase_sector(Handle, codeTableBlock);
        status = bspi_write(Handle, (int8_t *)&dataBuf[codeTableBufPtr], (uint64_t)codeTableBlock * spiInfo.BlockSize, codeTableLen);
        ASSERT(status == codeTableLen);
    }

    return OK;    
}
#endif

#endif


