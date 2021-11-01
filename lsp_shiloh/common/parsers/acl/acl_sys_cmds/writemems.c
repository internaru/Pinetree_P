/*
 *
 * ============================================================================
 * Copyright (c) 2007-2009   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file writemems.c 
 *
 */

#include <string.h>
#include <stdint.h>

#include "dprintf.h"
#include "logger.h"
#include "acl_api.h"
#include "regutils.h"
#ifdef HAVE_EEPROM
#include "eeprom_api.h"
#endif
#include "checksum.h"
#ifdef HAVE_BOOTSPI
#include "bootspi_api.h"
#endif
#include "checksum.h"
#include "acl_nvram_constants.h"

/*
 ** WriteMems
 *
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
ATSTATUS WriteMems( unsigned char *downBuf, uint32_t dataLength, uint8_t busType, bool fErase )
{
    ATSTATUS status=AT_SUCCESS;
    uint32_t crcVal, totalBytes;

#ifdef HAVE_BOOTSPI
    spi_device_t *spiHandle=NULL;
#endif

    //
    // Now do a CRC on all the data except the CRC and make sure the data is valid.
    //
    memcpy(&totalBytes, &downBuf[dataLength - CHECKSUM_LENGTH], CHECKSUM_LENGTH);
    crcVal = CalcChecksum((uint32_t *) downBuf, dataLength - CHECKSUM_LENGTH);


    if(crcVal == totalBytes)
    {   /* The checksum is valid, use it */
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACL: CheckSum Valid Write to %d\n", busType));


        switch(busType)
        {
#ifdef HAVE_BOOTSPI //SPIMEM
            case BUS_TYPE_SPI:

                if((status = bspi_open(MEM_SPI0, &spiHandle)) == OK)
                {

                    //status = SPIMemOverwriteDev(downBuf, dataLength - CHECKSUM_LENGTH, fErase);
                    status = bspi_write(spiHandle, (int8_t *)downBuf, 0, dataLength - CHECKSUM_LENGTH);
                    bspi_close(spiHandle);
                }

                else
                   DPRINTF(DBG_SOFT|DBG_OUTPUT, ("ACL: MEM_SPI0 open failed\n"));
                break;
#endif

#ifdef HAVE_EEPROM
            case BUS_TYPE_I2C:
            {
                EEPROM_DEVICE_t *devHandle = NULL;
                if((status = eepromOpen(MEM_I2C, &devHandle)) == OK)
                {
                    status = eepromWrite(devHandle, (uint8_t *)downBuf, 0, dataLength - CHECKSUM_LENGTH);
                    if(status == (dataLength - CHECKSUM_LENGTH))
                    {
                        status = AT_SUCCESS;
                    }
                    eepromClose(devHandle);
                }
                else
                {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("ACL: MEM_I2C open failed\n"));
                }
                break;
            }
#endif
            case BUS_TYPE_DIRECT:
            /* Purposely do nothing in this case */
            break;


            default:
                status = AT_FAILURE;
                break;
        }
    }
    else
    {
        status = AT_FAILURE;
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("ACL: CheckSum Failed\n"));

    }

    return status;
}

