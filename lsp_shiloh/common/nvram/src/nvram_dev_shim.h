/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file nvram_dev_shim.h
 *
 *  \brief External interface for NVRAM variable block device shim layer
 *
 *  This shim sits between the NV Manager and the Partition Manager and is 
 *  intended to hide device specific knowledge from the upper parts of the 
 *  application layer.  Architectural location of this file:
 *      +----------------------------------------------+
 *      |        NVRAM API (Application Layer)         |
 *      +----------------------------------------------+
 *      |            NV Device Shim Layer              |
 *      |                (THIS FILE)                   |
 *      +----------------------+-----------------------+
 *      |    EEPROM Support    |      SPI Support      |
 *      |                      |                       |
 *      +----------------------+-----------------------+
 *      |            Partition Manager API             |
 *      +----------------------------------------------+
 *      |          Memory Device Driver Layer          |
 *      +----------------------------------------------+
 *
 *  \warning
 *  The current implementation only supports managing a container partition on
 *  a single memory device: the device shim binds to a specific device at start
 *  up.
 *
 **/

#ifndef INC_NVRAM_DEV_SHIM_H
#define INC_NVRAM_DEV_SHIM_H

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "deviceid.h"
#include "nvram_api.h"

#ifdef __cplusplus
extern "C" {
#endif


#define VARBLOCK_PART_ID    0xABCD      /**< Container partition ID           */

#define INVALID_ENTRY       0xffffffff  /**< Invalid entry type flag          */

/**
 *  \brief NV Instance structure
 *
 *  This structure is returned to the client when they register a variable block:
 *  an instance handle is required to use most interfaces in this API.
 **/
typedef struct NVM_Handle_s
{
    char          BlockLabel[4];        /**< Block label: 4 ASCII characters  */
    uint32_t      MaxLength;            /**< Maximum length of the var block including header */
#ifdef HAVE_NVRAM_FILE
    int           fd;
#else
    MEMORY_DEVICE DeviceID;             /**< Device storing the var block     */
#endif
} PrivateNvramInstance;


/**
 *  \brief NV storage header
 *
 *  Every variable block placed into NV storage will have this header, followed
 *  by the actual block data (in Vars).  The header and the variables are
 *  stored in a single memory allocation (header size + Len).
 **/
typedef struct VARBLOCK_DATA_s
{
    uint32_t Version;             /**< Block version                        */
    int8_t   Vars[];              /**< 'Len' bytes of variable data         */
} VARBLOCK_DATA_t;

typedef struct VARBLOCK_HEADER_s
{
    uint32_t CRC;                 /**< Block CRC                            */
    uint32_t Len;                 /**< Length of the variable block         */
    VARBLOCK_DATA_t Data;         /**< CRC is calculated on this portion    */
} VARBLOCK_HEADER_t;


/* FUNCTION NAME: InitNvramDevShim */
/**
 *  \brief Called once at power up, this function initializes the device shim
 *
 *  This function needs to be called before any applications attempt to access
 *  the NV store.  This routine will:
 *   - attempt to locate an existing container partition in the search path, 
 *     creating one if needed
 *   - binds the device shim to a specific container
 * 
 *  \return error_type_t 
 *  \retval OK    Successfully bound to container partition
 *  \retval FAIL  Error creating/binding to container
 **/
error_type_t InitNvramDevShim(void);


/* FUNCTION NAME: LocateVarBlock */
/** 
 *  \brief Located a variable block in the container partition
 *
 *  This routine attempts to locate a variable block in the NV store (the
 *  container partition).  The caller allocates an nvram_handle_t structure
 *  and fills out the 'BlockLabel' field.  If the block is located, this routine
 *  will fill out the 'MaxLength' and 'DeviceID' fields in the instance structure.
 *
 *  NOTE: the caller cannot specify a memory device to search, the device shim
 *        binds to a single NV store at start up time.
 *
 *  \param[in]  *nHandle  Pointer NV instance handle, allocated by caller
 *
 *  \return error_type_t 
 *  \retval OK    Variable block found
 *  \retval FAIL  Variable block not found
 **/
error_type_t LocateVarBlock(nvram_handle_t *nHandle);


/* FUNCTION NAME: RegisterVarBlock */
/** 
 *  \brief Gets a handle to a variable block
 *
 *  This routine attempts to return a valid variable block handle to the caller. 
 *  The caller allocates an nvram_handle_t structure and fills out the 'BlockLabel' 
 *  and 'MaxLength' fields.  If an existing block cannot be located, a new one
 *  will be created.  This routine will fill out the 'DeviceID' field
 *  in the instance structure.
 *
 *  NOTE: the caller cannot specify a memory device, the device shim
 *        binds to a single NV store at start up time.
 *
 *  \warning
 *  The NV API is responsible for making sure the size of the variable block is
 *  correct.  If the sizes don't match, this routine will fail and the handle
 *  will not be usable.
 *
 *  \param[in]  *nHandle  Pointer NV instance handle, allocated by caller
 *
 *  \return error_type_t 
 *  \retval OK    Valid variable block handle returned
 *  \retval FAIL  Unable to locate or create variable block (or size mismatch)
 **/ 
error_type_t RegisterVarBlock(nvram_handle_t *nHandle);


/* FUNCTION NAME: ReleaseVarBlock */
/**
 *  \brief Removes a registered variable block
 *
 *  This routine removes a registered variable block from the NV store. 
 *
 *  \param[in] *nHandle   Valid handle obtained from the register call 
 *
 *  \return error_type_t 
 *  \retval OK    Variable block removed
 *  \retval FAIL  Variable block could not be removed
 **/ 
error_type_t ReleaseVarBlock(nvram_handle_t *nHandle);


/* FUNCTION NAME: VarBlockRead */
/**
 *  \brief Reads data from a registered variable block
 *
 *  This routine reads data from a registered variable block. Note that this
 *  command will allow the caller to read a subset of the block data by using
 *  the offset and size parameters.  Reading off the end of the var block is
 *  not allowed (no data will be returned).
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
uint32_t VarBlockRead(nvram_handle_t *nHandle, int8_t *blockPtr, uint64_t offset, uint32_t size);


/* FUNCTION NAME: VarBlockWrite */
/**
 *  \brief Writes a registered variable block
 *
 *  This routine writes data to a registered variable block. Note that this
 *  command will NOT allow the caller to write a subset of the block data, the
 *  entire block must be written (nHandle->MaxLength).
 *
 *  \param[in] *nHandle   Valid handle obtained from the register call
 *  \param[in] *blockPtr  Buffer pointer to data to be written, allocated by
 *                        the caller
 * 
 *  \return uint32_t 
 *  \retval 0 if problem writing the block (or parameters invalid)
 *  \retval Actual number of bytes written to block
 **/
uint32_t VarBlockWrite(nvram_handle_t *nHandle, int8_t *blockPtr);


/* FUNCTION NAME: VarBlockCheckCRC */
/**
 *  \brief Validates variable block CRC
 *
 *  This routine examines the CRC of the passed variable block.
 *
 *  \param[in] *label     4 character ascii block label 
 *  \param[in] *varBlock  Pointer to a variable block
 *  \param[in] erased_data_val Data value of erased memory for the type of memory being checked.
 *  Typically 0xFFFFFFFF for eeprom and spi flash.
 * 
 *  \return bool 
 *  \retval true  variable block CRC is OK
 *  \retval false bad CRC
 **/
bool VarBlockCheckCRC(char *label, VARBLOCK_HEADER_t *varBlock, uint32_t erased_data_val);



#ifdef __cplusplus
}
#endif

#endif /* INC_NVRAM_DEV_SHIM_H */
