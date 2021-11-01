/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file regmemdev.c
 *
 *  \brief File for registering available memory devices with memdevice.
 **/

#include <string.h>
#include <stdint.h>
#include "error_types.h"
#include "lassert.h"
#include "deviceid.h"
#include "memdevice.h"
#include "regmemdev.h"
#include "nvram_config.h"

#ifdef HAVE_I2C
#ifdef HAVE_EEPROM
  #include "eeprom_api.h"
#endif
#endif

#ifdef HAVE_BOOTSPI
  #include "bootspi_api.h"
#endif

#ifdef HAVE_NAND_NV
  #include "nand_api.h"
#endif

/**
 * \brief Register Available Memory Devices
 *
 * For each memory device defined in the build:
 *  - try and open the device (may work even if device not present)
 *  - try and read from the device
 *  - register the device if open and read worked
 *
 * NOTE: at some point in the future the device driver Open methods may
 *       verify the device is actually present (which means we don't
 *       have to do the read here).
 **/
#ifdef HAVE_I2C
#ifdef HAVE_EEPROM
static	ALIGN(32) uint32_t      readData;
#endif
#endif

void RegisterMemDrv(void)
{   
#ifdef HAVE_I2C
#ifdef HAVE_EEPROM
    {
		MEMDEV_HANDLE memDev;
		uint32_t      readLen = sizeof(uint32_t);
		EEPROM_DEVICE_t *i2cHandle;
     
        /* Attempt to open the I2C driver */
        if (eepromOpen(MEM_I2C, &i2cHandle) == OK)
        {
            /* Attempt to read data */            
            if (eepromRead(i2cHandle, (uint8_t *)&readData, 0, readLen) == readLen)
            {
                /* MEM_I2C exists, register the device */
                memset((char *)&memDev, 0, sizeof(MEMDEV_HANDLE));
                memDev.DeviceID  = MEM_I2C;
                memDev.Open      = (OpenFunc)  eepromOpen;
                memDev.Close     = (CloseFunc) eepromClose;
                memDev.Read      = (ReadFunc)  eepromRead;
                memDev.Write     = (WriteFunc) eepromWrite;
                memDev.Report    = (ReportFunc)eepromReport;
                memDev.Erase     = (EraseFunc) eepromErase;    
#ifdef NVRAM_SUPPORTS_AUTO_FORMAT
                memDev.Format    = nvconfig_eeprom_format;
                memDev.VerifyFormat = nvconfig_eeprom_verify_format;
#endif
                if (RegisterMemoryDevice(&memDev) != OK)
                {
                    ASSERT(false);
                }
            }
            
            /* Close the device */
            eepromClose(i2cHandle);
        }
    }
#endif
#endif

#ifdef HAVE_BOOTSPI
    {
		MEMDEV_HANDLE memDev;
		uint32_t      readData;
		uint32_t      readLen = sizeof(uint32_t);
        spi_device_t *spiHandle;
        
        /* Attempt to open the SPI 0 driver */
        if (bspi_open(MEM_SPI0, &spiHandle) == OK)
        {
            /* Attempt to read data */            
            if (bspi_read(spiHandle, (int8_t *)&readData, 0, readLen) == readLen)
            {
                /* MEM_SPI0 exists, register the device */
                memset((char *)&memDev, 0, sizeof(MEMDEV_HANDLE));
                memDev.DeviceID  = MEM_SPI0;
                memDev.Open      = (OpenFunc)   bspi_open;
                memDev.Close     = (CloseFunc)  bspi_close;
                memDev.Read      = (ReadFunc)   bspi_read;
                memDev.Write     = (WriteFunc)  bspi_write;
                memDev.Erase     = (EraseFunc)  bspi_erase_sector;
                memDev.Report    = (ReportFunc) bspi_report;
#ifdef NVRAM_SUPPORTS_AUTO_FORMAT
                memDev.Format    = nvconfig_bspi_format;
                memDev.VerifyFormat = nvconfig_bspi_verify_format;
#endif
              
                if (RegisterMemoryDevice(&memDev) != OK)
                {
                    ASSERT(false);
                }    
            }

            /* Close the device */
            bspi_close(spiHandle);
        }
     
        // MEM_SPI1 not supported yet
    }
#endif

#ifdef HAVE_NAND_NV
    {
        MEMDEV_HANDLE memDev;
        nand_device_t  *nandHandle;
    
        /* Attempt to open the NAND 0 driver */
        if (nand_open(MEM_NAND0, &nandHandle) == OK)
        {
            /* no known value gained from reading memory - except if debug situation...
             * would need to read with ECC disabled, or ignore the error */

            /* MEM_NAND0 exists, register the device */
            memset((char *)&memDev, 0, sizeof(MEMDEV_HANDLE));
            memDev.DeviceID  = MEM_NAND0;
            memDev.Open      = (OpenFunc)   nand_open;
            memDev.Close     = (CloseFunc)  nand_close;
            memDev.Read      = (ReadFunc)   nand_read;
            memDev.Write     = (WriteFunc)  nand_write;
            memDev.Erase     = (EraseFunc)  nand_erase_block;
            memDev.Report    = (ReportFunc) nand_report;
#ifdef NVRAM_SUPPORTS_AUTO_FORMAT
            memDev.Format    = nvconfig_nand_format;
            memDev.VerifyFormat = nvconfig_nand_verify_format;
#endif

            if (RegisterMemoryDevice(&memDev) != OK)
            {
                ASSERT(false);
            }

            /* Close the device */        
            nand_close(nandHandle);
        }
 
        // MEM_NAND1 not supported yet
    }    
#endif
}
