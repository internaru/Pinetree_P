/*
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *  \file imageMapUtil.h
 *
 *  \brief Image map utility interface definitions
 *
 *  The Image Map (or Code Table) completely describes the objects stored on a
 *  non-volatile memory device.  The image map is created by our tools and then
 *  burned to the memory part: the image map is then treated as read only by
 *  our mainline firmware.
 *
 *  Things get a bit complicated because we have several possible image map
 *  formats (usually because of constraints imposed by boot code in ROM).  The 
 *  NVM config layer will define which version we are currently dealing with
 *  (IMAGEMAP_VER_major_minor), but all versions must support a common subset
 *  of fields or we will not be able to isolate the differences within this file.
 *
 *  The BLOCK_NODE_t structure is used by the caller to get image map entry
 *  data: not all fields will be used.
 *
 *  Note that the image map version is a \#define so that we can compile out
 *  unused versions to save space.
 **/

#ifndef INC_IMAGEMAPUTIL_H
#define INC_IMAGEMAPUTIL_H

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "nvram_config.h"
#include "deviceid.h"
#include "nodeCache.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Image map constants (version specific)
 */
#if defined(IMAGEMAP_VER_0_0)
 
  #define CODE_KEY            0xC0DEED0C   /**< ROM Code Table signature          */
  #define MAX_CODE_TABLE_LEN  2048          /**< Maximum code table length         */
  #define INVALID_CT_ENTRY    0xffffffff   /**< Invalid code table entry type/id  */

#elif defined(IMAGEMAP_VER_1_0)

  #define CODE_KEY            0xC1DEED1C   /**< ROM Image Map signature           */

#else
    #error "Image Map Version Not Defined (see nvram_config.h)"
#endif
 
 
/**
 *  \struct ImageMapInfo_t
 *
 *  \brief Image map information
 *
 *  Data returned to the client after an image map is located and validated.  
 *  This structure common across all image map versions, so all fields may
 *  not actually come from image map (may be set by firmware depending
 *  on the NVM configuration).
 **/
typedef struct ImageMapInfo_s
{
    MEMORY_DEVICE  MemDev;          /**< Memory device storing the image map  */
    bool           validImageMap;   /**< Image map valid on MemDev            */
    uint64_t       imOffset;        /**< IM offset in bytes from part addr 0  */
    uint32_t       imLenBytes;      /**< IM length in BYTES (includes hdr)    */
    uint32_t       imHdrLen;        /**< Length of IM header in BYTES         */
    uint32_t       imEntryLen;      /**< Length of each IM entry in BYTES     */
    uint16_t       imMajorVer;      /**< Image map major version              */
    uint16_t       imMinorVer;      /**< Image map minor version              */
    uint32_t       imNumEntries;    /**< Number of entries in the image map   */
    uint32_t       imTerm;          /**< Image map terminator (if used)       */
} ImageMapInfo_t;


/**
 *  \struct ImageMapHeader_t
 *
 *  \brief Image map header structure
 *
 *  The image map header directly precedes the image map entries on the memory device.
 *  Note that the length of this header must always be an even number of 32 bit words.
 **/
#if defined(IMAGEMAP_VER_0_0)

    typedef struct ImageMapHeader_s
    {
        uint32_t   Signature;       /**< Image map signature                  */
    } code_table_header_t;

#elif defined(IMAGEMAP_VER_1_0)
#if 0
    typedef struct ImageMapHeader_s
    {
        uint32_t   Signature;       /**< Image map signature                  */
        uint32_t   CRC;             /**< Image map CRC (all fields after CRC) */
        uint32_t   Length;          /**< Total length (including header)      */
        uint16_t   MajorVersion;    /**< Major version of the image map       */
        uint16_t   MinorVersion;    /**< Minor version of the image map       */
    } ImageMapHeader_t;
#endif
#else
    #error "Image Map Version Not Defined (see nvram_config.h)"
#endif


/**
 *  \struct ImageMapEntry_t
 *
 *  \brief Image map entry structure
 *
 *  Defines an image map entry. Note that the length of this structure must
 *  always be an even number of 32 bit words.
 **/
#if defined(IMAGEMAP_VER_0_0)

    typedef struct ImageMapEntry_s
    {
        uint32_t   Offset;          /**< Object offset (bytes from IM start)  */
        uint32_t   Length;          /**< Object length in bytes               */
        uint16_t   ID;              /**< Object identifier                    */
        uint16_t   Type;            /**< Object type (ImageMapType_t)         */
    } code_table_entry_t;

#elif defined(IMAGEMAP_VER_1_0)
#if 0
    typedef struct ImageMapEntry_s
    {
        uint32_t   ID;              /**< Object identifier                    */
        uint16_t   Type;            /**< Object type (ImageMapType_t)         */
        uint16_t   Flags;           /**< Object flags field                   */
        uint32_t   HighOffset;      /**< Object offset (bytes from IM start)  */
        uint32_t   LowOffset;       /**< Object offset (bytes from IM start)  */
        uint32_t   HighLength;      /**< Object length in bytes - top 32 bits */
        uint32_t   LowLength;       /**< Object length in bytes - low 32 bits */
        uint32_t   Version;         /**< Optional object version              */
        uint32_t   Reserved2;       /**< Reserved for future use              */
    } ImageMapEntry_t;
#endif
#else
    #error "Image Map Version Not Defined (see nvram_config.h)"
#endif


/**
 *  \enum ImageMapType_e
 *
 *  \brief Image map type enumeration
 *
 *  Defines the type of an image map entry.
 **/
  typedef enum ImageMapType_e
  {
      IM_CFG_SECTOR = 1,       /**< system configuration table               */
      IM_CODE_SECTOR,          /**< executable code (firmware)               */
      IM_DATA_SECTOR,          /**< binary data (must have data header)      */
      IM_SPARE_SECTOR,         /**< not used: still here for boot code       */
      IM_NV_STORE,             /**< NV memory data store (container)         */
      IM_FILE_SYS_STORE        /**< file system information                  */
  } ImageMapType_t;




/**
 *  \brief Find and validate Image Map on specified memory device
 *
 *  This function searches the specified memory device for a valid image map
 *  and returns information in the provided ImageMapInfo_t structure.
 *
 *  \param[in] memDev    Memory device to search
 *  \param[in] *imInfo   Pointer to image map info structure, allocated by caller
 *
 *  \return error_type_t 
 *  \retval  OK if valid image map found
 *  \retval  FAIL if image map not found or memory device error
 **/
error_type_t FindImageMap(MEMORY_DEVICE memDev, ImageMapInfo_t *imInfo);


/**
 *  \brief Read the specified image entry
 *
 *  This function reads the specified image map entry (the entry header, not the
 *  object data) into a generic BLOCK_NODE_t structure.  This isolates the caller
 *  from the specific image entry format.
 *
 *  \param[in] *imInfo    Pointer to image map info structure, filled out by the
 *                        FindImageMap() routine
 *  \param[in] *devInfo   Pointer to device information structure 
 *  \param[in] *imBuffer  A RAM buffer containing the entire image map (allocated
 *                        and filled by caller)
 *  \param[in]  entryNum  Entry number to read (first entry is 0)
 *  \param[out] *imEntry  Pointer to BLOCK_NODE_t structure, allocated by caller
 *
 *  \return error_type_t 
 *  \retval  OK if entry found
 *  \retval  FAIL if entry not found
 **/
error_type_t ReadImageMapEntry(ImageMapInfo_t  *imInfo,
                               DRIVER_CONFIG_t *devInfo,
                               int8_t          *imBuffer,
                               uint32_t         entryNum,
                               BLOCK_NODE_t    *imEntry);



#ifdef __cplusplus
}
#endif

#endif /* INC_IMAGEMAPUTIL_H */
