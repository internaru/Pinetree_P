/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file rommgrapi.c
 *
 *  \brief Implementation for the ROM Manager.
 *
 *  The ROM Manager handles the retrieval of static data from ROM.  Note that 
 *  this API only supports CONFIG_TABLE and DATA object types.
 *
 **/

#include <string.h>
#include <stdint.h>
#include "error_types.h"
#include "lassert.h"
#include "memAPI.h"
#include "dprintf.h"
#include "logger.h"
#include "deviceid.h"
#include "partitionMgrAPI.h"
#include "rommgrapi.h"
#include "nvram_config.h"
#include "checksum.h"
#include "ioutils.h"
#include "cpu_api.h"

/* Settings for debug messages */
#ifndef DBGOPT
  #define DBGOPT (DBG_LOUD | DBG_TERMINAL)
#endif

/*
 *  Local variable types
 */
typedef enum Data_Compress_Type_e
{
    eNoCompression = 0,
    eGzipCompressed
} Data_Compress_Type_t;


typedef struct Data_Img_Hdr_s
{
    uint32_t ID;                ///< ID (as stored in the ROM)
    uint32_t Length;            ///< Length as stored in memory part
    uint32_t UnCompressedLen;   ///< Size of uncompressed image in ROM excluding header size
    uint32_t CompressType;      ///< Compression type flag 
    uint32_t CRC;               ///< Data CRC
    int8_t   Data[];            ///< The actual data (Length bytes)
} Data_Img_Hdr_t;


/*
 *  Local (static) functions
 */
static error_type_t deCompressData(int8_t   *CompressedData, 
                                   uint32_t  CompressedSize, 
                                   int8_t   *UnCompressedData,
                                   uint32_t  UnCompressedSize);


/*
 *  Function: RomMgrReadData
 *
 *  See rommgrapi.h for function documentation.
 */
int8_t *RomMgrReadData(ImageMapType_t  BlockType, 
                       uint32_t        ID, 
                       uint32_t       *ImageSize)
{
    uint32_t             len;
    uint32_t             dataLen;
    int8_t              *OutData;
    Data_Img_Hdr_t      *DataHdr;
    int8_t              *rawData;
    error_type_t         decompStatus;
    MEMORY_DEVICE        targetDev;
    partition_handle_t  *Handle;
    partition_info_t     Info;
    PARTMGR_ERROR_t      readErr;
    uint32_t             crc;
    const MEMORY_DEVICE *searchList;

    /* Set the return image size in case we fail */
    *ImageSize = 0;

    /* The ROM Manager only deals with a couple of data block types:
     *  - CONFIG_DATA (uncompressed data with no header)
     *  - DATA        (binary data with image header, may be compressed)
     */
    if ((BlockType != IM_CFG_SECTOR) && (BlockType != IM_DATA_SECTOR))
    {
        DPRINTF(DBGOPT, ("RomMgr: Unsupported block type (%d)\n", BlockType));        
        return NULL;
    } 
    
    /*
     * Find our partition using the device search list from the NVM config layer.
     */
    searchList = getRommgrSearchList();
    targetDev  = SearchForPartition(ID, BlockType, searchList);
    if (targetDev == MEM_DEVICE_NOT_FOUND)
    {
        DPRINTF(DBGOPT, ("RomMgr: Unable to find ID=%d Type=%d \n", ID, BlockType));
        return NULL;
    }
         
    /* Try and open the partition. The open call will fail if the partition
     * is not found on the specified device.
     */
    if (PARTMGR_OK != OpenPartition(ID, BlockType, targetDev, &Handle))
    {
        DPRINTF(DBGOPT, ("RomMgr: Unable to open ID=%d Type=%d \n", ID, BlockType));
        return NULL;
    } 

    /* Get partition information (we need the size to alloc memory) */
    if (PARTMGR_OK != GetPartitionInfoByHandle(Handle, &Info))
    {
       ClosePartition(Handle);
       return NULL;
    }

    /* Allocate a buffer for the raw partition data */
    rawData = (int8_t *)MEM_MALLOC_ALIGN(Info.Size, cpu_get_dcache_line_size());
    if (rawData == NULL)
    {
        ClosePartition(Handle);
        DPRINTF(DBGOPT, ("RomMgr: Unable to allocate memory for ram buffer\n"));
        return NULL;
    }

    /* Read all the data into our RAM buffer */
    readErr = ReadPartition(Handle, rawData, 0, Info.Size, &len);
    if ((readErr != PARTMGR_OK) || (len != Info.Size))
    {
        DPRINTF(DBGOPT, ("RomMgr: Unable to read data from memory\n"));
        MEM_FREE_AND_NULL(rawData);
        ClosePartition(Handle);
        return NULL;
    }

    /* Don't need the partition open any more, close it */
    ClosePartition(Handle);

    /* Just return the raw data if it is a config table (no header and no compression) */ 
    if (BlockType == IM_CFG_SECTOR)
    {
        *ImageSize = len;
        return rawData;
    }   

    /* If we made it here we have a Data Image Header.  We will need to allocate a
     * return buffer (an unaltered pointer from a malloc), the size will depend on
     * whether we have to decompress.
     */
    DataHdr = (Data_Img_Hdr_t *)rawData;
    if (be32_to_cpu(DataHdr->CompressType) == eNoCompression)
    {
        dataLen = be32_to_cpu(DataHdr->Length);
    }
    else
    {
        dataLen = be32_to_cpu(DataHdr->UnCompressedLen);
    }
    crc = CalcChecksum((uint32_t *)DataHdr->Data, be32_to_cpu(DataHdr->Length));
    if(crc != be32_to_cpu(DataHdr->CRC))
    {
        DPRINTF(DBGOPT,("Failed crc\n"));
        memFree(rawData);
        return NULL;
    }
    OutData = (int8_t *)MEM_MALLOC_ALIGN(dataLen, cpu_get_dcache_line_size());
    if (OutData == NULL)
    {
        DPRINTF(DBGOPT, ("RomMgr: Unable to allocate memory for ram buffer\n"));
        MEM_FREE_AND_NULL(rawData);
        return NULL;
    }
 
    if (be32_to_cpu(DataHdr->CompressType) == eNoCompression)
    {
        if (be32_to_cpu(DataHdr->Length) > (Info.Size - sizeof(Data_Img_Hdr_t)))
        {
            /* Not enough data! */
            ASSERT(false);
        }

        /* Just move the raw data into the out buffer */
        memcpy(OutData, &DataHdr->Data[0], dataLen);   
    }
    else
    {
        decompStatus = deCompressData((int8_t*)&DataHdr->Data[0], be32_to_cpu(DataHdr->Length), 
                                      (int8_t *)OutData, be32_to_cpu(DataHdr->UnCompressedLen));
        if (decompStatus != OK)
        {
            DPRINTF(DBGOPT, ("RomMgr: Unable to decompress the data\n"));
            MEM_FREE_AND_NULL(rawData);
            MEM_FREE_AND_NULL(OutData);
            return NULL;
        }
    }
    
    /* Free the raw data buffer and exit */
    MEM_FREE_AND_NULL(rawData);
    *ImageSize = dataLen;
    return OutData;
}

/*
 *  Function: RomMgrWriteData
 *
 *  See rommgrapi.h for function documentation.
 */
error_type_t RomMgrWriteData(ImageMapType_t   BlockType, 
                             uint32_t         ID, 
                             int8_t          *Image, 
                             uint32_t         ImageSize)
{
    error_type_t         e_res;
    uint32_t             len;
    MEMORY_DEVICE        targetDev;
    partition_handle_t  *Handle;
    partition_info_t     Info;
    PARTMGR_ERROR_t      p_res;
    const MEMORY_DEVICE *searchList;

    e_res = OK;
    Handle = NULL;

    /* apparently we wonly support burning config table's right now */
    if (BlockType != IM_CFG_SECTOR)
    {
        DPRINTF(DBGOPT, ("RomMgr: Unsupported block type (%d)\n", BlockType));        
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /*
         * Find our partition using the device search list from the config
         * layer.
         */
        searchList = getRommgrSearchList();
        targetDev  = SearchForPartition(ID, BlockType, searchList);
        if (targetDev == MEM_DEVICE_NOT_FOUND)
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* Try and open the partition. The open call will fail if the partition
         * is not found on the specified device.
         */
        if (PARTMGR_OK != OpenPartition(ID, BlockType, targetDev, &Handle))
        {
            DPRINTF(DBGOPT, ("RomMgr: Unable to find Block=%d \n", BlockType));
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* Get partition information (we need the size to alloc memory) */
        if (PARTMGR_OK != GetPartitionInfoByHandle(Handle, &Info))
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* Just write the data if it is a config table (they are supposed to be stored
         * uncompressed with no header)
         */
        ASSERT(BlockType == IM_CFG_SECTOR);

        /* some non-volatile memories need to be erased before writing
         * do that here */
        p_res = ErasePartition( Handle, 
                                0, 
                                Info.Size );

        if ( PARTMGR_OK != p_res )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        p_res = WritePartition(Handle, (int8_t *)Image, 0, Info.Size, &len);
        if ((p_res != PARTMGR_OK) || (len != Info.Size))
        {
            DPRINTF(DBGOPT, ("RomMgr: Unable to write data \n"));
            e_res = FAIL;
        }
    }

    if ( NULL != Handle )
    {
        ClosePartition(Handle);
    }

    return e_res;
}


#ifdef HAVE_ZLIB

#include "zconf.h"              /* required for data compression */
#include "zlib.h"               /* required for data compression */
voidpf zalloc(void *opaque, uInt size, uInt objects)
{
    return MEM_MALLOC(size * objects);
}
void zfree(void *opaque, void *address)
{
    memFree(address);
}

/**
 *  \brief Returns the uncompressed data in destination buffer.
 *
 *  \param[in] CompressedData    Pointer to compressed data
 *  \param[in] CompressedSize    Length of compressed data
 *
 *  \param[in] UnCompressedData  Pointer to destination data buffer
 *  \param[in] UnCompressedSize  Length of uncompressed data
 *
 *  \return  error_type_t
 *  \retval  OK   - on success
 *  \retval  FAIL - on Failure
 **/
static error_type_t deCompressData(int8_t   *CompressedData, 
                                   uint32_t  CompressedSize, 
                                   int8_t   *UnCompressedData, 
                                   uint32_t  UnCompressedSize)
{
    int      err;
    z_stream d_stream;

    /* Un-compress the data into the buffer */

    d_stream.zalloc = zalloc;
    d_stream.zfree = zfree;
    d_stream.opaque = NULL;

    err = inflateInit(&d_stream);
    if (err != Z_OK)
    {
         DPRINTF(DBGOPT, ("RomMgr: Unable to init decompressor\n"));
         return(FAIL);
    }

    d_stream.next_out  = (Bytef *)UnCompressedData;
    d_stream.avail_out = UnCompressedSize;

    d_stream.next_in  = (Bytef *)CompressedData;
    d_stream.avail_in = CompressedSize;

    err = inflate(&d_stream, Z_NO_FLUSH);
    if (err == Z_STREAM_END)
    {
        /* make sure we got the expected amount of data */
        if ((d_stream.avail_in != 0) ||
            (d_stream.total_out != UnCompressedSize))
        {
            DPRINTF(DBGOPT, ("RomMgr: De-compress failed\n"));
            return FAIL;
        }
    }
    else
    {
        DPRINTF(DBGOPT, ("RomMgr: Inflate returned an error\n"));
        return FAIL;
    }

    err = inflateEnd(&d_stream);
    if (err != Z_OK)
    {
         DPRINTF(DBGOPT, ("RomMgr: Unable to close decompressor\n"));
         return(FAIL);
    }
   
    return OK;
}
#else
static error_type_t deCompressData(int8_t   *CompressedData, 
                                   uint32_t  CompressedSize, 
                                   int8_t   *UnCompressedData, 
                                   uint32_t  UnCompressedSize)
{
  return FAIL; /* we cannot decompress with zlib */
}
#endif
