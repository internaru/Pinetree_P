/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_DEVICEID_H
#define INC_DEVICEID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enumerates all the memory devices that may be present in a system.  Notes:
 *  - MEM_I2C must always be first in the list
 *  - MEM_DEVICE_NOT_FOUND must always be last in the list
 *  - The list must start at 0 (I2C) and remain sequential
 *  - MEMORY_DEVICE must match mem_dev_msg strings (see deviceid.c)
 */
typedef enum
{
    MEM_I2C = 0,   ///< I2C bus, EEPROM
    MEM_RTC,       ///< Real Time Clock
    MEM_SPI0,      ///< BootSPI NVRAM
    MEM_SPI1,
    MEM_SPI2,
    MEM_SPI3,
    MEM_SPI4,
    MEM_NAND0,     ///< NAND NVRAM
    MEM_NAND1,
    MEM_DEVICE_NOT_FOUND
} MEMORY_DEVICE;


/**
 * \brief This structure is common for all devices doing a report
 */
typedef struct
{
    uint32_t    PageSize;
    uint32_t    BlockSize;
    uint32_t    BlockCount;
    uint32_t    ECCSize;
} DRIVER_CONFIG_t;


/**
 *  \brief Get string version of memory device enum
 *
 *  Utility function to assist with printing human readable debug messages. 
 *
 *  \param[in] memDev  Memory device enum value
 **/
const char *getDevIdStr(MEMORY_DEVICE memDev);

#ifdef __cplusplus
}
#endif

#endif /* INC_DEVICEID_H */
