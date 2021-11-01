/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_MEMDEVICE_H
#define INC_MEMDEVICE_H

/**
 *  \file memdevice.h
 *
 *  \brief Memory Device Driver Manager
 *
 *  Device drivers register with this manager which allows a single point of
 *  entry for accessing memory device drivers.
 *
 *  Note that all addresses used by this API are 'virtual' device addresses:
 *  the caller must convert from partition relative addresses.
 *
 *  Multiple users can call devOpen() and each receives mutex protection from 
 *  the inividual device drivers.
 **/

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"
#include "deviceid.h"
#include "ATypes.h"      /* for DEVICE_HANDLE */
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * All abstracted device drivers must provide the following interfaces:
 */
typedef error_type_t (*OpenFunc)   (MEMORY_DEVICE, DEVICE_HANDLE *);
typedef error_type_t (*CloseFunc)  (DEVICE_HANDLE);
typedef int32_t      (*ReadFunc)   (DEVICE_HANDLE, int8_t *, uint64_t, uint32_t);
typedef int32_t      (*WriteFunc)  (DEVICE_HANDLE, int8_t *, uint64_t, uint32_t);
typedef error_type_t (*ReportFunc) (DEVICE_HANDLE, DRIVER_CONFIG_t *Config);
typedef error_type_t (*EraseFunc)  (DEVICE_HANDLE, uint32_t);

/*
 * Optionally format and verfication routines may be provided. This is part and product specific and is 
 * typically implemented in the product's nvram_config.c/h files 
 */
typedef error_type_t (*FormatFunc) (DEVICE_HANDLE);
typedef error_type_t (*VerifyFormatFunc) (uint8_t*);

/*
 * Our low-level device abstraction structure
 */
typedef struct
{
    ATLISTENTRY         node;           /**< node for linking MEMDEV_HANDLE into a list */
    MEMORY_DEVICE       DeviceID;       /**< Memory device ID                         */
    OpenFunc            Open;           /**< Pointer to Device's Open() function      */
    CloseFunc           Close;          /**< Pointer to Device's Close() function     */
    ReadFunc            Read;           /**< Pointer to Device's Read() function      */
    WriteFunc           Write;          /**< Pointer to Device's Write() function     */
    ReportFunc          Report;         /**< Pointer to Device's Report() function    */
    EraseFunc           Erase;          /**< Pointer to Device's Erase() Block/Sector */
    FormatFunc          Format;         /**< Pointer to Device's Format function (if any) */
    VerifyFormatFunc    VerifyFormat;   /**< Pointer to Device's Verify Format function (if any) */
    DEVICE_HANDLE *MemHandle;      /**< Driver allocated memory handle           */
} MEMDEV_HANDLE;


/* FUNCTION NAME: InitMemDeviceAPI */
/**
 *  \brief Called to give the deviceAPI a chance to set itself up
 **/
void InitMemDeviceAPI(void);


/* FUNCTION NAME: RegisterMemoryDevice */
/**
 *  \brief Register Memory Device
 *
 *  Allows a memory device to register and be accessed by the memdevice gateway
 * 
 *  \param[in] *Handle  A handle filled in with device ID and function addresses
 *
 *  \return error_type_t
 **/
error_type_t RegisterMemoryDevice(MEMDEV_HANDLE *memDev);


/* FUNCTION NAME: devOpen */
/**
 *  \brief Attempt to open a device that was previously registered
 *
 *  \param[in]  DeviceID  Specifies the memory device
 *  \param[out] **Handle  A pointer to a memory location that receives the 
 *                        handle pointer
 *
 *  \return error_type_t
 **/
error_type_t devOpen(MEMORY_DEVICE DeviceID, MEMDEV_HANDLE **Handle);


/* FUNCTION NAME: devClose */
/**
 *  \brief Close a previously opened device.
 *
 *  \param[in] Handle The handle returned from devOpen()
 *
 *  \return error_type_t
 **/
error_type_t devClose(MEMDEV_HANDLE *Handle);


/* FUNCTION NAME: devReport */
/**
 *  \brief Reports on the current device's block count and block size.
 *
 *  \param[in] Handle   The handle received via devOpen()
 *  \param[in] devData  Pointer to device configuration structure, allocated
 *                      by the caller
 *
 *  \return error_type_t
 **/
error_type_t devReport(MEMDEV_HANDLE *Handle, DRIVER_CONFIG_t *devData);


/* FUNCTION NAME: devRead */
/**
 *  \brief Call the device read function
 *
 *  \param[in] *Handle The handle returned from a previous open command.
 *  \param[in] *Dest   A pointer to the destination for the read bytes.
 *  \param[in] Address The (device virtual) address to start reading from
 *  \param[in] Length  The number of bytes to read
 *
 *  \return  int32_t
 *  \retval  Actual number of bytes read, 0 if error
 **/
int32_t devRead(MEMDEV_HANDLE *Handle, int8_t *Dest, uint64_t Address, uint32_t Length);


/* FUNCTION NAME: devWrite */
/**
 *  \brief Call the device write function
 *
 *  \param[in] *Handle The handle returned from a previous open command.
 *  \param[in] *Src    A pointer to the source of the data to be written
 *  \param[in] Address The (device virtual) address to start writing
 *  \param[in] Length  The number of bytes to write
 *
 *  \return  int32_t
 *  \retval  Actual number of bytes written, 0 if error
 **/
int32_t devWrite(MEMDEV_HANDLE *Handle, int8_t *Src, uint64_t Address, uint32_t Length);


/* FUNCTION NAME: devErase */
/**
 *  \brief Call the device Erase function
 *
 *  \param[in] *Handle The handle returned from a previous open command
 *  \param[in] Block   The number of the block/sector to be erased
 *
 *  \return error_type_t
 **/
error_type_t devErase(MEMDEV_HANDLE *Handle, uint32_t Block);


/* FUNCTION NAME: memDevInstalled */
/**
 *  \brief Is the specifed memory device installed?
 *
 *  \param[in] DeviceID device ID enum
 *
 *  \return bool
 *  \retval true=installed, false=not installed
 **/
bool memDevInstalled(MEMORY_DEVICE DeviceID);


/* FUNCTION NAME: devFormat */
/**
 *  \brief Call the device Format function.  This creates a config table
 *  on the device.  Note that this is a destructive operation.  Note that
 *  not all devices support this routine.  Devices which do not support
 *  the routine will return FAIL.
 *
 *  \param[in] *Handle The handle returned from a previous open command
 *
 *  \return error_type_t
 **/
error_type_t devFormat(MEMDEV_HANDLE *Handle);

/* FUNCTION NAME: devVerifyNvFormat */
/**
 *  \brief Call the device Format Verfication function.  This is verifies
 *  that the passed in config table buffer is valid for the particular
 *  platform.  The buffer passed in must contain a known valid config table
 *  that has passed length and checksum tests.  Note that
 *  not all devices support this routine.  Devices which do not support
 *  the routine will return OK.
 *
 *  \param[in] *ctbuffer - pointer to a buffer containing a known valid
 *                          config table
 *
 *  \return error_type_t
 **/
error_type_t devVerifyNvFormat(MEMDEV_HANDLE *Handle, uint8_t* ctbuffer);

#ifdef __cplusplus
}
#endif

#endif /* INC_MEMDEVICE_H */
