/**
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_NVRAM_DEV_EEPROM_H
#define INC_NVRAM_DEV_EEPROM_H

/**
 *  \file nvram_dev_eeprom.h
 *
 *  \brief External interface for NVRAM variable block storage on EEPROM devices
 *
 *  This shim sits between the NV Manager and the Partition Manager and is 
 *  intended to hide EEPROM specific knowledge from the upper parts of the 
 *  application layer.  Architectural location of this file:
 *      +---------------------------------------------------+
 *      |            NVRAM API (Application Layer)          |
 *      +---------------------------------------------------+
 *      |                NV Device Shim Layer               |
 *      +------------------+---------------+----------------+
 *      |  EEPROM Support  |  SPI Support  |  NAND Support  |
 *      |    (THIS FILE)   |               |                |
 *      +------------------+---------------+----------------+
 *      |                Partition Manager API              |
 *      +---------------------------------------------------+
 *      |              Memory Device Driver Layer           |
 *      +---------------------------------------------------+
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "deviceid.h"
#include "nvram_api.h"

#ifdef __cplusplus
extern "C" {
#endif


#define REMOVE    0            /**< Remove var block flag    */
#define ADD       1            /**< Add var block flag       */


/* FUNCTION NAME: InitNvramEEPROM */
/**
 *  \brief Initialize the NV EEPROM device shim
 *
 *  Must be called before using any of the other EEPROM device shim routines.
 *
 **/
void InitNvramEEPROM(void);


/* FUNCTION NAME: GetPartitionNeedsEEPROM */
/**
 *  \brief Return requirements for the EEPROM based container partition
 *
 *  This routine returns the container partition requirements for the specified
 *  memory device.
 *
 *  \param[in]  memDev         Memory device to host container partition
 *
 *  \param[out] *partitionSize Pointer to the returned container partition size
 *                             (in bytes)
 * 
 *  \return error_type_t 
 *  \retval OK    Returned container values valid for the specified memory device
 *  \retval FAIL  Container values not valid, container can't be placed on the 
 *                specified memory device
 **/
error_type_t GetPartitionNeedsEEPROM(MEMORY_DEVICE      memDev, 
                                     uint32_t          *partitionSize);
                                     


/* FUNCTION NAME: DiscoverVarBlocksEEPROM */
/**
 *  \brief Discovers existing variable blocks on the specified device
 *
 *  This routine parses the container partition on the specified device
 *  and builds a directory of valid variable blocks.  This should be
 *  called once at start-up or when recovering from certain error
 *  conditions.
 *
 *  \warning
 *  The container partition must be validated prior to calling this routine:
 *    - created on specified device if it doesn't exist
 *    - verify that it meets needs from GetPartitionNeedsEEPROM(): a 
 *      partition that does not will cause data corruption/loss
 *
 *  \param[in]  memDev       Memory device to host container partition
 * 
 *  \return error_type_t 
 *  \retval OK    Container successfully parsed
 *  \retval FAIL  Problem with container, no parsing done
 **/
void DiscoverVarBlocksEEPROM(MEMORY_DEVICE memDev);


/* FUNCTION NAME: GetVarBlockSizeEEPROM */
/**
 *  \brief Returns the size of the named variable block
 *
 *  This routine attempts to locate the variable block by label on the
 *  specified device and return the var block size.   This routine can also
 *  be used to test for the existence of a variable block.
 *
 *  \param[in]  memDev   Memory device to search
 *  \param[in]  *label   Pointer to a 4 character ASCII block label 
 * 
 *  \return uint32_t 
 *  \retval Size of var block in bytes or 0 if not found
 **/
uint32_t GetVarBlockSizeEEPROM(MEMORY_DEVICE memDev, char *label);


/* FUNCTION NAME: AddRmVarBlockEEPROM */
/**
 *  \brief Adds or removes a block of NV variables
 *
 *  This routine adds or removes a block of NV variables.
 *
 *  \param[in]  *label   Pointer to a 4 character ASCII block label 
 *  \param[in]  memDev   Memory device to host variables
 *  \param[in]  maxSize  Size of variable block
 *  \param[in]  IsAdd    1 for ADD, 0 for REMOVE
 * 
 *  \return error_type_t 
 *  \retval OK    Operation successful
 *  \retval FAIL  Operation failed
 **/
error_type_t AddRmVarBlockEEPROM(char          *label, 
                                 MEMORY_DEVICE  memDev,
                                 uint32_t       maxSize, 
                                 bool           IsAdd);


/* FUNCTION NAME: VarBlockReadEEPROM */
/**
 *  \brief Reads data from a registered variable block
 *
 *  This routine reads data from a registered variable block. Note that this
 *  command will allow the caller to read a subset of the block data by using
 *  the offset and size parameters.  Reading off the end of the var block is
 *  not allowed (no data will be returned).
 *
 *  \warning
 *  You cannot read a block until it is committed in the NV store.
 *
 *  \param[in] *nHandle   Valid handle obtained from the register call
 *  \param[in] *blockPtr  Buffer pointer to receive the read data.  Allocated
 *                        by the caller, must be at least 'size' bytes or
 *                        memory will be corrupted
 *  \param[in] offset     Offset from var block start to begin reading
 *  \param[in] size       Number of bytes to read
 * 
 *  \return uint32_t 
 *  \retval 0 if problem reading block (or parameters invalid)
 *  \retval Actual number of bytes read from block
 **/
uint32_t VarBlockReadEEPROM(nvram_handle_t   *nHandle, 
                            int8_t         *blockPtr,
                            uint64_t        offset, 
                            uint32_t        size);


/* FUNCTION NAME: VarBlockWriteEEPROM */
/**
 *  \brief Writes a registered variable block
 *
 *  This routine writes data to a registered variable block. Note that this
 *  command will NOT allow the caller to write a subset of the block data, the
 *  entire block must be written (nHandle->MaxLength).
 *
 *  \param[in] *nHandle   Valid handle obtained from the register call
 *  \param[in] *Src       Buffer pointer to data to be written, allocated by
 *                        the caller
  * 
 *  \return uint32_t 
 *  \retval 0 if problem writing the block (or parameters invalid)
 *  \retval Actual number of bytes written to block
 **/
uint32_t VarBlockWriteEEPROM(nvram_handle_t   *nHandle, 
                             int8_t         *Src);
                          

#ifdef __cplusplus
}
#endif

#endif /* INC_NVRAM_DEV_EEPROM_H */
