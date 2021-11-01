/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief Implements support for the ACL language.
 *
 */

#ifndef INC_ACL_NVRAM_CONSTANTS_H
#define INC_ACL_NVRAM_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 BurnFLASH command information

 The high bit of the destination field is used to indicate if the part needs to
 be erased before burning.  If the bit is clear, the part will be erased before
 burning if the bit is set, there will be no erase before the burn.

**/
#define BF_NO_ERASE         0x8000
#define BF_ACL_FILE         0x4000          // ACL_FILE bit to know not to USB respond
#define BF_NO_REBOOT        0x2000          // REQUEST to NOT Reboot upon completion
#define BF_MASK             0xE000

///<  These are the possible destinations for the burn flash command
#define BF_SPI0             0x0400          ///<  write to spi0
#define BF_SPI1             0x0500          ///<  write to spi1
#define BF_SPI2             0x0600          ///<  write to spi2
#define BF_SPI3             0x0700          ///<  write to spi3
#define BF_I2C              0x0100          ///<  write to the i2c part
#define BF_MARVELL_WIRELESS 0x0004          ///<  Marvell/Zcom wireless module SPI part

#define BUS_TYPE_SPI    0
#define BUS_TYPE_I2C    1
#define BUS_TYPE_DIRECT 2

#define CHECKSUM_LENGTH 4


#ifdef __cplusplus
}
#endif

#endif

