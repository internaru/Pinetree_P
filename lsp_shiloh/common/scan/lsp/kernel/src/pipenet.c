/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/*
 * pipenet.c, linux kernel edition
 *
 * pipenet is deprecated and will soon be removed. Has been superceded by
 * icetest+icefile. This file is only here to allow current code to compile.
 *
 * davep 16-Oct-2012
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */

#include "scos.h"

#include "list.h"

#include "scantypes.h"
#include "scancore.h"
#include "safetylock.h"
#include "icedma.h"

bool pipenet_message( scan_msg_t *msg, uint32_t *wait_flags )
{
    return false;
}

void pipenet_init( void )
{
}

int pipenet_multichannel_open( 
                   uint8_t read_channels[],
                   uint8_t num_read_channels,

                   uint8_t write_channels[],
                   uint8_t num_write_channels,

                   int total_rows_in,
                   int total_rows_out,
                   int read_buffers_per_channel,
                   int read_rows_per_buffer,
                   int read_bytes_per_row )
{
    return -1;
}

void pipenet_rdma_buffer( struct ice_dma_buffer **icebuf )
{
    BUG();
}

void pipenet_multichannel_close( void )
{

}

