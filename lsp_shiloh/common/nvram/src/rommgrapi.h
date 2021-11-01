/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_ROMMGRAPI_H
#define INC_ROMMGRAPI_H

/**
 *  \file rommgrapi.h
 *
 *  \brief API header for the ROM Manager.
 *
 *  The ROM Manager handles the retrieval of static objects from NV memory. Note
 *  that this API only deals with a couple of data block types:
 *    - CONFIG_DATA (uncompressed data with no header)
 *    - DATA        (binary data with image header, may be compressed)
 *
 *  Architectural location of this file:
 *      +---------------------------------------------------+
 *      |            ROMMGR API (Application Layer)         |
 *      |                    (THIS FILE)                    |
 *      +---------------------------------------------------+
 *      |                Partition Manager API              |
 *      +---------------------------------------------------+
 *      |              Memory Device Driver Layer           |
 *      +---------------------------------------------------+
 
 **/

#include <stdint.h>
#include "error_types.h"
#include "nvram_config.h"
#include "partitionMgrAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *  \brief Returns the specified data stored in NV memory
 *
 *  Memory devices are searched in the order specified in the NV config layer,
 *  the highest priority instance of this object is returned.
 *
 *  \param[in]  BlockType   Type of data block to be Read
 *  \param[in]  ID          Specific data block ID
 *  \param[out] *ImageSize  Pointer to size of returned data block
 *                          (0 returned on error)
 *
 *  \return int8_t*
 *  \retval Pointer to returned data buffer (allocated by RomMgrReadData)
 *  \retval NULL on error
 **/
int8_t *RomMgrReadData(ImageMapType_t   BlockType, 
                       uint32_t         ID, 
                       uint32_t        *ImageSize);


/**
 *  \brief Writes a block of data back to a static area of memory. 
 *
 *  Writing data back to ROM?  This should only be done in the field
 *  replaceable entity use cases!
 *
 *  \warning
 *  We currently only support replacing the config table, but that is
 *  probably a bad idea.
 * 
 *  \param[in] BlockType   Type of data block to be written
 *  \param[in] ID          Specific block ID
 *  \param[in] *Image      Pointer to source data buffer
 *  \param[in] ImageSize   Length of the source buffer
 *
 *  \return error_type_t
 *  \retval OK - success
 *  \retval FAIL - on error
 **/
error_type_t RomMgrWriteData(ImageMapType_t   BlockType, 
                             uint32_t         ID, 
                             int8_t          *Image, 
                             uint32_t         ImageSize);


#ifdef __cplusplus
}
#endif

#endif /* INC_ROMMGRAPI_H */

