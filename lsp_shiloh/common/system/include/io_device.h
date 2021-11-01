/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file io_device.h
 *
 * \brief Interface for io's that provide read write ioctl interfaces as a pipe.
 * usb print endpoint or port 9100 or scan ports for instance.
 *
 */

#ifndef IO_DEVICE_H
#define IO_DEVICE_H

#include <stdint.h>

/** ioctl commands. */
typedef enum 
{
    e_close = 1,  ///< Upper level close, io error or timeout will cause this, pipeconnect required for next read.
    e_close_write, ///< Writing done for this job.
    /// The following are all used by usb mass storage device.
    e_STALLIN,          ///< Stall the in endpoint.
    e_STALLOUT,         ///< Stall the out endpoint.

    e_IOInterface,  ///< Which i/o interface is this pipe associated with?

    /// the following are currently not used or implemented.
    e_StatusReport,
    e_WaitForWrites,
    e_ResetWrites,       ///< Wipe out all pending writes and abort current write.
    e_ClearAllWrites,     ///< Get rid of all writes for all endpoints.
    e_TransferActive,    ///< Returns a 1 if something has not xfered since last time we called.
    e_WaitForNaks        ///< Waits for a nak on the in endpoint.  Ptr val is timeout in ticks.
} ioctl_cmd_t ;

typedef enum 
{
    e_parallel = 1,
    e_usb,
    e_network,
    e_internal_page,
}IOPort;

/**
 * IO pipes and parser must have matching types of channel functionality print
 * to print, scan to scan; often ports have associated behavior, 9100, tcp,
 * usbprint endpoint...
 */
typedef enum 
{
    e_Print,  ///< Port 9100 or usb print for example.
    e_Scan,
    e_MassStorage,  // Doesn't use CM
    e_HTTP,         // Doesn't use CM
    e_FaxChan,
    e_PictBridgeChan,
    e_12844,        // Use for 1284.4 print support.
    e_Cmd,          // Doesn't use CM
    e_peg_passthru
} Channel_type_t;


/**
 * \brief These are return codes from the io read and write functions.
 *
 * - + If the result is greater than 0, the return value is the number of bytes
 * read or written.
 * - - Negative returns indicate socket closed or timeout close.
 * - 0 Returns are common when polling a connection and no data is available
 * right then.
 */
#define CONIO_NODATA   0 ///< 0 or no data available on the connection
#define CONIO_CONDONE -1 ///< Connection is done (normal close, returned by 2BYREQUEST connections)
#define CONIO_CONERR  -2 ///< Connection error (abnormal close, returned by BYREQUEST)
#define CONIO_TIMEOUT -3 

/** 
 * Opaque structure pointer for the lower level device to cast into its
 * internal data type.
 */
typedef struct io_device_impl_s io_device_impl_t;


/**
 * \brief IO structure for pipe like devices.
 *
 * Data channel is the common read write pipe that something like port 9100
 * uses.
 * Command channel is less common ftp uses both a data and a command channel.
 * USB interrupts can use the cmd_write.  cmd_read is currently unused.
 */
typedef struct io_device_handle_s 
{
    /// IO data channel
    int32_t (*data_read) (io_device_impl_t *fh, void *DataBuffer, uint32_t Length, uint32_t TimeOut); ///< Read function
    int32_t (*data_write) (io_device_impl_t *fh, void *DataBuffer, uint32_t Length, uint32_t Timeout);     ///< Write function      
    /// IO command channel
    int32_t (*cmd_read) (io_device_impl_t *fh, void *DataBuffer, uint32_t Length, uint32_t TimeOut); ///< Read function
    int32_t (*cmd_write) (io_device_impl_t *fh, void *DataBuffer, uint32_t Length, uint32_t Timeout); ///< Write function usb interrupt     
    int32_t (*ioctl) (io_device_impl_t *fh, ioctl_cmd_t Cmd, void *Details);       ///< IOctal interface to driver

    io_device_impl_t *data_read_handle; ///< File handle for read, Private
    io_device_impl_t *data_write_handle; ///< File handle fo write, Private.
    io_device_impl_t *cmd_read_handle; ///< File handle for cmd read, unsused . Private.
    io_device_impl_t *cmd_write_handle;///< File handle for cmd write, Private.

} io_device_handle_t;

#endif // IO_DEVICE_H
