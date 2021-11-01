/*
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PIPE_IO_DEVICE_H
#define PIPE_IO_DEVICE_H

#include "io_device.h"

int32_t pipe_io_device_fh_pipe_write(io_device_impl_t *fh, 
                                     void *DataBuffer, 
                                     uint32_t Length, 
                                     uint32_t TimeOut ///< centi seconds
                                     );

int32_t pipe_io_device_fh_pipe_read(io_device_impl_t *fh, 
                                    void *DataBuffer,
                                    uint32_t Length, 
                                    uint32_t TimeOut ///< centi seconds
                                    );

int32_t pipe_io_device_fh_pipe_ioctl(io_device_impl_t *fh, 
                                     ioctl_cmd_t Cmd, 
                                     void *Details);      ///< IOctal interface to driver

/// attach a file for read/write to both the data and the cmd.  
int pipe_io_device_register( const char * readfile, const char *statusfile, Channel_type_t channel_type );

///
/// sysinit function: uses a thread to break context between the client callers and the underlying file/pipe
void pipe_io_device_init(void);

#endif
