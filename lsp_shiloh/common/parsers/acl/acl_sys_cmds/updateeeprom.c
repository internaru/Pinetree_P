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
 * \file updateeeprom.c
 *
 * \brief Update single locations in the config table.
 *
 */

#include <stdint.h>

#include "boardCfg.h"
#include "dprintf.h"

#include "regDefs.h"
#include "nvram.h"
#include "agConnectMgr.h"
#include "memAPI.h"

#include "agiLogger.h"
#include "aclAPI.h"
#include "regutils.h"
#include "eepromapi.h"
#include "checksum.h"
#include "lassert.h"



/* FUNCTION NAME: AclEEPROMUpdate */

/**
 * \brief Update eeprom values.
     This command allows the user to specify the number of bytes to
     * be written, and the offset to write them to. The first 16 bits
     * of data is the byte count, the next 16 bits is the offset into
     * the nvram part. The data to be
     * written follows the size and offset values.  This should be deprecated.

 *
 *
 * \param[in] Pipe  IO Pipe for communication with the host.
 *
 * \param[in] AclCmd The command that caused this to be called.
 *
 * \param reserved Save for later.
 *
 * \retval SINT32 0 for success, 1 for failure.
 *
 * \date 2/27/2006
 *
 **/
int32_t AclEEPROMUpdate(IOReg *Pipe, ACL_CMD *AclCmd, void *reserved)
{

    /* This command allows the user to specify the number of bytes to
     * be written, and the offset to write them to. The first 16 bits
     * of data is the byte count, the next 16 bits is the offset into
     * the nvram part. The data to be
     * written follows the size and offset values.
     */
    int16_t offset;
    unsigned char *DataBuf, *ChksumBuf;
    uint32_t promEmptyCode = 0xffffffff, checksum, size;
    int32_t bytesRead, bytesWritten;
    ATSTATUS status=AT_SUCCESS;
    ACL_RESPONSE Buffer;
    EEPROM_DEVICE_t *devHandle = NULL;

    InitACLResponse(&Buffer);

    DataBuf = NULL;
    ChksumBuf = NULL;

    size = AclCmd->data.updateNVRam.nvramLength;
    offset = AclCmd->data.updateNVRam.nvramOffset;

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL update nvram: size = %u, offset = %u\n", size, offset ));

    /* Now, read the number of bytes specified by the user */
    DataBuf = (unsigned char *)MEM_MALLOC( size );
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Download is %d bytes\n", size));

    if ( DataBuf != NULL )
    {
        bytesRead = Pipe->ReadConnect(Pipe, DataBuf, size, PARSEREAD_EXACT );
        if (bytesRead < 0)
            bytesRead = 0;      // force an error if the read failed.
    }
    else
    {
        bytesRead = 0;
    }

    if (bytesRead != size)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Wrong number of bytes read, abort\n"));
        status = AT_FAILURE;

        if (DataBuf != NULL)
        {
            MEM_FREE_AND_NULL(DataBuf);
        }

        return -1;
    }

    //disable datastore writes while we update the part
 //   DSDisableWriteBack();

    /* Now write the values to the EEPROM. Set the checksum
     * invalid, then write the new values. Finally, calculate a
     * new checksum and write it as well. The location of the
     * checksum depends on whether the static or dynamic area
     * is being written!
     */

    //Open I2C device
    if((status = eepromOpen(MEM_I2C, &devHandle)) != OK)
    {
        if (DataBuf != NULL)
        {
            MEM_FREE_AND_NULL(DataBuf);
        }
        return - 1;
    }
    if (offset > NVRAM_STATIC_DATA_SIZE)
    {
        /* Get a buffer to read nvram contents into, for
         * calculating new checksum after the write completes.
         */
        ChksumBuf = (unsigned char *) MEM_MALLOC( DYNAMIC_NVRAM_SIZE );

        DPRINTF(DBG_LOUD | DBG_OUTPUT, ("Writing dynamic area!\n"));
        /* Invalidate dynamic area checksum. Checksum is located 4 bytes
         * from the end of the part.
         */
        status = eepromWrite(devHandle, (uint8_t *)promEmptyCode,
                          NVRAM_STATIC_DATA_SIZE + DYNAMIC_NVRAM_SIZE - 4, 4);
        if(status ==  4)
            status = AT_SUCCESS;
    }
    else
    {
        /* Get a buffer to read nvram contents into, for
         * calculating new checksum after the write completes.
         */
        ChksumBuf = (unsigned char *) MEM_MALLOC( NVRAM_STATIC_DATA_SIZE );

        DPRINTF(DBG_LOUD | DBG_OUTPUT, ("Writing static area!\n"));
        /* Invalidate static area checksum */
        status = eepromWrite(devHandle, (uint8_t *)promEmptyCode, 0, 2);
        if(status == 2)
            status = AT_SUCCESS;

    }

    /* Okay, write the data to the specified nvram location */
    DPRINTF(DBG_LOUD | DBG_OUTPUT, ("Writing %s\n", DataBuf));
    if(status == AT_SUCCESS)
    {
       status = eepromWrite(devHandle, (uint8_t *)DataBuf, offset, size);
       if(status == size)
           status = AT_SUCCESS;
    }

       // status = RWI2CEEProm((UINT8*)DataBuf, offset, size, EEPROM_WRITE);

    /* Now, read the data portion of the NVRAM and calculate
     * a new checksum. Note that a checksum is maintained
     * for the static and dynamic nvram areas. The static area
     * checksum is stored at location 0, whereas the dynamic
     * area checksum is stored in the last 4 bytes of the
     * dynamic area. CAUTION! This depends on correct definition
     * of the NVRAM_DYNAMIC_DATA_SIZE constant!
     */
    if (offset > NVRAM_STATIC_DATA_SIZE)
    {
        /* Read from the end of the static area, which comes first in
         * the nvram part, to the end of the dynamic area, leaving
         * room for the checksum.
         */
        status = eepromRead(devHandle, (uint8_t*)ChksumBuf, NVRAM_STATIC_DATA_SIZE, DYNAMIC_NVRAM_SIZE -4);
        checksum = CalcChecksum((uint32_t*)ChksumBuf, DYNAMIC_NVRAM_SIZE-4);
        status = eepromWrite(devHandle, (uint8_t*)checksum, NVRAM_STATIC_DATA_SIZE + DYNAMIC_NVRAM_SIZE -4, 4);
        if(status == 4)
            status = AT_SUCCESS;

    }
    else
    {
        /* Remember, checksum comes first in the static area! */
        status = eepromRead(devHandle, (uint8_t*)ChksumBuf, 2, NVRAM_STATIC_DATA_SIZE-2);
        checksum = CalcChecksum((uint32_t *)ChksumBuf, NVRAM_STATIC_DATA_SIZE-2);
        status = eepromWrite(devHandle, (uint8_t*)&checksum+2, 0, 2);
        if(status == 2)
            status = AT_SUCCESS;

    }

    //reenable write backs
 //   DSEnableWriteBack();

    if (status != AT_SUCCESS)
    {
        Buffer.cmdStatus = ACLCMD_STATUS_FAILURE;
    }
    Buffer.cmdID = ACLCMD_UPDATE_EEPROM;

    bytesWritten = Send16ByteACLResponse(Pipe, &Buffer);
    if (DataBuf != NULL)
    {
        MEM_FREE_AND_NULL(DataBuf);
    }
    if (ChksumBuf != NULL)
    {
        MEM_FREE_AND_NULL(ChksumBuf);
    }
    if( NULL != devHandle )
    {
        eepromClose(devHandle);
    }
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", bytesWritten));
    return 0;
}

