/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file partitionMgrAPI.h
 *
 * \brief External interface definitions for the Partition Manager API
 *
 * The partition manager allows applications to share space on a physical
 * non-volatile storage device (EEPROM, SPI Flash, NAND, etc.) by managing one or
 * more partitions.
 *
 * Partition information is stored on each physical device in a 'code pointer table'
 * or 'Image Map' per the Boot ROM ERS. At a minimum, each partition must have:
 *  - offset (logical device address, relative to the code table)
 *  - size   (length in bytes)
 *  - id     (entry identifier)
 *  - type   (entry type)
 *
 * The design intent is to have all the NV entities (partitions) fully specified 
 * in the devices original 'burn image'.  In essence this means that the devices 
 * address space is fully defined by the provided code table: the code table will
 * have a fixed size and be treated as read-only.
 *
 * In this light a CreatePartition() or DeletePartition() API is not provided.
 *
 * NOTES:
 *  - Addresses within a partition are always relative to the beginning of the 
 *    partition (not the physical device).
 *  - The partition manager simply manages partitions on the physical device: 
 *    what happens in the partition space is the applications sole responsibility.  
 *    This includes things like wear leveling schemes.
 *  - Don't mix the use of partition manager and device driver 'direct'
 *    applications on the same physical device: data corruption could result.
 *  - Entities may be packed into the address space (not aligned to page/sector/block
 *    boundaries).
 *  - Partitions pointed to by the code table may not be fully initialized by the
 *    burn image; the firmware may set up the partitioned areas at run time (the 
 *    address space is reserved by the code table).
 *
 **/

#ifndef INC_PARTITIONMGRAPI_H
#define INC_PARTITIONMGRAPI_H

#include <stdint.h>
#include <stdbool.h>
#include "nvram_config.h"
#include "deviceid.h"
#include "imageMapUtil.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * API type definitions
 */

/**
 * \brief Enumerated Partition Manager API function return codes
 **/ 
typedef enum PARTMGR_ERROR_e
{
    PARTMGR_OK = 0,          /**< API call OK                                */
    PARTMGR_FAIL,            /**< Generic/unknown failure                    */
    PARTMGR_PARMERR,         /**< Function parameter error                   */
    PARTMGR_EXISTS,          /**< Requested partition already exists         */
    PARTMGR_NOTFOUND,        /**< Requested partition not found              */
    PARTMGR_NODEVICE,        /**< Requested device not installed/registered  */
    PARTMGR_DEVERROR,        /**< Device read/write error                    */
    PARTMGR_MEMERR,          /**< Unable to allocate memory                  */
    PARTMGR_LENERROR,        /**< Attempt to read/write past partition end   */
    PARTMGR_OPEN             /**< Partition is already open                  */
} PARTMGR_ERROR_t;

/**
 * \brief Info structure returned by the get partition information functions
 **/ 
typedef struct partition_info_s
{
    MEMORY_DEVICE      Dev;         /**< Enumerated memory device for partition   */
    uint64_t           StartOffset; /**< Partition start offset (device relative) */
    uint64_t           Size;        /**< Size of partition in bytes               */
    uint32_t           Flags;       /**< Partition specific flags                 */
    uint32_t           Version;     /**< Optional product specific version field  */
} partition_info_t;

/**
 * \brief Partition handle returned by partition open commands
 **/ 
typedef struct partition_handle_s partition_handle_t;


/*
 *  API Function declarations
 */

/* FUNCTION NAME: InitPartitionManager */

/**
 *  \brief Called once at power up, this function initializes the partition manager
 *
 *  This function needs to be called from main():
 *   - after the physical devices are registered with memdevice
 *   - before any partition manager based applications are started
 *  
 *  This routine will parse the code table on registered physical devices 
 *  and attempt to identify existing partitions.
 *
 *  NOTES:
 *   - this routine generally asserts when problems are encountered as we are 
 *     running at boot time (no recovery is possible) 
 **/
void InitPartitionManager(void);


/* FUNCTION NAME: SearchForPartition */

/**
 *  \brief Finds the highest priority partition instance by ID and Type
 *
 *  Searches the provided memory device search list for an instance of the
 *  specified partition (by ID and Type). The search list must be ordered 
 *  from highest to lowest priority and terminated with MEM_DEVICE_NOT_FOUND 
 *  (see deviceid.h).
 *
 *  \param[in]  ID           Object identifier (numeric or 4 ASCII chars)
 *  \param[in]  Type         Enumerated object type
 *  \param[in]  *searchList  Memory device search list
 *
 *  \return MEMORY_DEVICE
 *  \retval Highest priority memory device containing partition
 *  \retval MEM_DEVICE_NOT_FOUND if partition not found
 **/
MEMORY_DEVICE SearchForPartition(uint32_t              ID,
                                 ImageMapType_t        Type,
                                 const MEMORY_DEVICE  *searchList);


/* FUNCTION NAME: GetPartitionInfo */

/**
 *  \brief Returns information on the specified partition
 *
 *  Applications can use this function to get general information on an existing
 *  partition, the partition need not be open.  The partition is located by
 *  ID, type, and memory device.
 *  
 *  This function can also be used to test for the existence of a partition by
 *  examining the return value (PARTMGR_OK versus PARTMGR_NOTFOUND).
 *
 *  \param[in]  ID     Object identifier (numeric or 4 ASCII chars)
 *  \param[in]  Type   Enumerated object type
 *  \param[in]  Dev    An enumerated memory device identifier
 *  \param[out] *Info  Pointer to a partition_info_t structure (caller must
 *                     allocate the structure memory) 
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition was found on specified device
 *  \retval PARTMGR_NOTFOUND No sign of the partition on specified device
 **/
PARTMGR_ERROR_t GetPartitionInfo(uint32_t          ID,
                                 ImageMapType_t    Type,
                                 MEMORY_DEVICE     Dev,
                                 partition_info_t *Info);


/* FUNCTION NAME: GetPartitionInfoByHandle */

/**
 *  \brief Returns information on the specified partition
 *
 *  Applications can use this function to get general information on an existing
 *  partition if you already have an open partition handle.
 *
 *  \param[in]  *Handle Pointer to open partition handle
 *  \param[out] *Info   Pointer to a partition_info_t structure (caller must
 *                      allocate the structure memory) 
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition data returned from handle
 **/
PARTMGR_ERROR_t GetPartitionInfoByHandle(partition_handle_t *Handle, 
                                         partition_info_t   *Info);


/* FUNCTION NAME: GetDeviceInfo */

/**
 *  \brief Returns information on the specified memory device
 *
 *  Because applications are completely responsible for managing the
 *  data in their partitions (wear leveling, erase before write, etc),
 *  they sometimes need details on the underlying storage device.
 *  
 *  This function can also be used to test for the existence of a device
 *  (PARTMGR_OK versus PARTMGR_NODEVICE).
 *
 *  \param[in]  Dev         Enumerated memory device identifier
 *  \param[out] *blockCount pointer to returned block count
 *  \param[out] *blockSize  pointer to returned block size 
 *  \param[out] *pageSize   pointer to returned page size 
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Device information successfully returned
 *  \retval PARTMGR_NODEVICE Device not installed or registered
 *  \retval PARTMGR_DEVERROR Unable to communicate with device driver
 **/
PARTMGR_ERROR_t GetDeviceInfo(MEMORY_DEVICE Dev, 
                              uint32_t     *blockCount, 
                              uint32_t     *blockSize,
                              uint32_t     *pageSize);


/* FUNCTION NAME: OpenPartition */

/**
 *  \brief Opens an existing partition on the specified device
 *
 *  Returns a partition pointer that can be used to read/write/erase
 *  partition data. The partition is located by device, ID, and Type.
 *
 *  \param[in]  ID     Object identifier (numeric or 4 ASCII chars)
 *  \param[in]  Type   Enumerated object type
 *  \param[in]  Dev    The device containing the partition
 *  \param[out] **Handle pointer to partition handle pointer; returned 
 *                       partition handle is used to access partition data
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition successfully opened
 *  \retval PARTMGR_NOTFOUND Partition not found on selected device
 *  \retval PARTMGR_MEMERR   Unable to allocate needed memory
 **/
PARTMGR_ERROR_t OpenPartition(uint32_t             ID,
                              ImageMapType_t       Type, 
                              MEMORY_DEVICE        Dev, 
                              partition_handle_t **Handle);


/* FUNCTION NAME: ClosePartition */

/**
 *  \brief Closes an open partition
 *
 *  Allows the partition manager to free any resources associated
 *  with an open partition handle.
 *
 *  \param[in] *Handle  pointer to valid (open) partition handle
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition successfully closed
 **/
PARTMGR_ERROR_t ClosePartition(partition_handle_t *Handle);


/* FUNCTION NAME: ReadPartition */

/**
 *  \brief Read data from an open partition
 *
 *  Reads data from a previously opened partition.  Note that all addresses
 *  are relative to the start of the partition (not the physical device). 
 *  Reading off the end of the partition will result in an error condition 
 * (no data will be returned).
 *
 *  \param[in] *Handle   Pointer to an open partition handle (created using the
 *                       OpenPartition method) 
 *  \param[in] *Dest     Pointer to a destination buffer to store the bytes read 
 *                       from the partition.
 *  \param[in]  Address  The partition relative address to start reading from
 *  \param[in]  Length   The number of bytes to read
 *  \param[out] *bytesRead Pointer to the actual number of bytes read
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition read successful
 *  \retval PARTMGR_LENERROR Attempt to read past end of partition
 *  \retval PARTMGR_DEVERROR Can't open device driver or read failed
 **/
PARTMGR_ERROR_t ReadPartition(partition_handle_t *Handle, 
                              int8_t             *Dest, 
                              uint64_t            Address, 
                              uint32_t            Length,
                              uint32_t           *bytesRead);


/* FUNCTION NAME: WritePartition */

/**
 *  \brief Write data to an open partition
 *
 *  Writes data to a previously opened partition.  Note that all addresses
 *  are relative to the start of the partition (not the physical device).  
 *  Attempts to write past the end of a partition will result in an error 
 *  condition (no data will be written).
 * 
 *  \warning 
 *  Some devices only allow writing data to addresses that are in an erased
 *  state.  It is currently up to the caller to ensure the device addresses
 *  are in the proper state for writing.
 *
 *  \param[in] *Handle  Pointer to an open partition handle (created using the 
 *                      OpenPartition method)
 *  \param[in] *Src     Pointer to a source buffer containing the bytes to write 
 *                      to the partition.
 *  \param[in] Address  The partition relative address to start writing to
 *  \param[in] Length   The number of bytes to write
 *  \param[out] *bytesWritten Pointer to the actual number of bytes written
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Partition write successful
 *  \retval PARTMGR_LENERROR Attempt to write past end of partition
 *  \retval PARTMGR_DEVERROR Can't open device driver or write failed
 **/
PARTMGR_ERROR_t WritePartition(partition_handle_t *Handle, 
                               int8_t             *Src, 
                               uint64_t            Address, 
                               uint32_t            Length,
                               uint32_t           *bytesWritten);


/* FUNCTION NAME: ErasePartition */

/**
 *  \brief Erase data from an open partition
 *
 *  Erases data from a previously opened partition.  Note that all addresses
 *  are relative to the start of the partition.  
 *
 *  \warning
 *  Because many devices only permit erasing on block/sector boundaries, the
 *  address and length parameters passed to this function must currently be
 *  aligned to block/sector boundaries.  Erasing becomes impossible when 
 *  memory is packed and a sector/block is shared by more than one partition.
 *
 *  \param[in] *Handle  Pointer to an open partition handle (created using the 
 *                      OpenPartition method) 
 *  \param[in] Address  The partition relative address to start erasing from
 *                      (currently must be block/sector aligned)
 *  \param[in] Length   The number of bytes to erase
 *                      (currently must be multiple of block/sector size)
 *
 *  \return PARTMGR_ERROR_t (API function return code enumeration)
 *  \retval PARTMGR_OK       Erase successful
 *  \retval PARTMGR_DEVERROR Unable to open device driver or erase fail
 *  \retval PARTMGR_PARMERR  Input parameters not properly aligned
 **/
PARTMGR_ERROR_t ErasePartition(partition_handle_t *Handle, 
                               uint64_t            Address, 
                               uint32_t            Length);


#ifdef __cplusplus
}
#endif

#endif /* INC_PARTITIONMGRAPI_H */
